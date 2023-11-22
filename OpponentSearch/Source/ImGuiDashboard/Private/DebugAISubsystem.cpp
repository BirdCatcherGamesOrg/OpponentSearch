// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.


#include "DebugAISubsystem.h"

#include "DebugDashboard.h"
#include "DebugDashboardConstants.h"
#include "EngineUtils.h"
#include "imgui.h"
#include "ImGuiDashboardLogCategory.h"
#include "ProceduralMeshComponent.h"
#include "SharedSearchSpaceDeveloperSettings.h"
#include "NativeSharedSearchSpaceLogicComponent.h"
#include "SharedSearchSpaceAgent.h"
#include "SharedSearchSpaceSubsystem.h"
#include "XLog.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	const FString DefaultPreviewValue = TEXT("Select...");
	
	template
	<typename Selectable, typename Transform>
	bool ComboBuilder(const FString& Label, const TArray<Selectable> Selectables, Transform SelectableTransform, const int32 CurrentSelection, int32& OutNewSelection)
	{
		bool bValueChanged = false;
		FString PreviewValue = DefaultPreviewValue;
		if (Selectables.IsValidIndex(CurrentSelection))
		{
			PreviewValue = SelectableTransform(*Selectables[CurrentSelection]);
		}
		if (ImGui::BeginCombo(TCHAR_TO_ANSI(*Label), TCHAR_TO_ANSI(*PreviewValue)))
		{
			for (int32 i = 0; i < Selectables.Num(); i++)
			{
				bool bSelected = i == CurrentSelection;
				FString Selection = SelectableTransform(*Selectables[i]);
				if (ImGui::Selectable(TCHAR_TO_ANSI(*Selection), &bSelected))
				{
					OutNewSelection = i;
					bValueChanged = true;
				}
				if (bSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		return bValueChanged;
	}
}

void UDebugAISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	LOG_INITIALIZE_SUBSYSTEM(LogImGuiDashboard);
	Super::Initialize(Collection);
}

void UDebugAISubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	BasicShapeMaterial = LoadObject<UMaterialInstance>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial_Inst.BasicShapeMaterial_Inst"));
	USharedSearchSpaceSubsystem* SharedPatrolWaypointsSubsystem =
		GetWorldRef().GetSubsystem<USharedSearchSpaceSubsystem>();
	SharedPatrolWaypointsSubsystem->OnTrackedNavMeshesUpdated().AddWeakLambda(this, [this](const ARecastNavMesh& RecastNavMesh)
	{
		RecastNavMeshes.Add(&RecastNavMesh);
	});
	UDebugDashboard* DebugDashboard = InWorld.GetSubsystem<UDebugDashboard>();
	FShowPanelSignature OnShowPanel = FShowPanelSignature::CreateWeakLambda(this, [this]()
	{
		ShowDashboard();
	});
	DebugDashboard->RegisterForShow(TEXT("AI"), OnShowPanel);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.ObjectFlags = RF_Transient;
	SegmentInformationComponentHolder = InWorld.SpawnActor<AActor>(SpawnParameters);

	const UDebugDashboardConstants* Constants = GetDefault<UDebugDashboardConstants>();
	FloatingTextWidgetType = Constants->FloatingTextWidgetType;

	const USharedSearchSpaceDeveloperSettings* PatrolBehaviorDeveloperSettings = GetDefault<USharedSearchSpaceDeveloperSettings>();
	PatrolTickRate = PatrolBehaviorDeveloperSettings->PatrolTickRate;

	TArray<TObjectPtr<const ARecastNavMesh>> TrackedRecastNavMeshes;
	SharedPatrolWaypointsSubsystem->GetTrackedNavMeshes(TrackedRecastNavMeshes);
	RecastNavMeshes.Append(TrackedRecastNavMeshes);

	FakePatroller = NewObject<USharedSearchSpaceAgent>(this);
}

void UDebugAISubsystem::ShowDashboard()
{
	if (RecastNavMeshes.Num() == 0)
	{
		ImGui::Text("No NavMeshes in the Waypoints Subsystem...");
		if (ImGui::Button("Discover NavMeshes?"))
		{
			for (TActorIterator<ARecastNavMesh> It(GetWorld()); It; ++It)
			{
				USharedSearchSpaceSubsystem* SharedPatrolWaypointsSubsystem =
					GetWorldRef().GetSubsystem<USharedSearchSpaceSubsystem>();
				const ARecastNavMesh* RecastNavMesh = *It;
				SharedPatrolWaypointsSubsystem->CreateSearchSpaces(*FakePatroller, *RecastNavMesh);
			}
		}
		return;
	}
	
	if (ImGui::InputDouble("Set Patrol Tick Rate", &PatrolTickRate))
	{
		USharedSearchSpaceSubsystem* SharedPatrolWaypointsSubsystem =
			GetWorldRef().GetSubsystem<USharedSearchSpaceSubsystem>();
		SharedPatrolWaypointsSubsystem->SetSearchUpdateTickRate(PatrolTickRate);
	}
	
	int32 NewSelection = -1;
	if (ComboBuilder("NavMeshes", RecastNavMeshes, [](const ARecastNavMesh& R) { return R.GetName(); }, CurrentSelection, NewSelection))
	{
		CurrentSelection = NewSelection;
	}
	
	if (!RecastNavMeshes.IsValidIndex(CurrentSelection))
	{
		return;
	}
	
	if (ImGui::Checkbox("Show Waypoints", &bShowWaypoints))
	{
		
	}
	
	if (ImGui::Checkbox("Show Paths", &bShowPaths))
	{
		
		USharedSearchSpaceSubsystem* SharedPatrolWaypointsSubsystem =
			GetWorldRef().GetSubsystem<USharedSearchSpaceSubsystem>();
		TArray<UNativeSharedSearchSpaceLogicComponent*> Patrollers;
		//SharedPatrolWaypointsSubsystem->GetPatrollers(*RecastNavMeshes[CurrentSelection], Patrollers);
		if (bShowPaths)
		{
			for (UNativeSharedSearchSpaceLogicComponent* Patroller : Patrollers)
			{
				FDelegateHandle OnPathCorridorUpdatedHandle = Patroller->OnPathCorridorUpdated().AddWeakLambda(this, [this]( const TArray<NavNodeRef>& PathCorridor)
				{
					FColor DebugColor = FColor::MakeRandomColor();
					for (const NavNodeRef& NavPathPoint : PathCorridor)
					{
						FVector PolyCenter;
						RecastNavMeshes[CurrentSelection]->GetPolyCenter(NavPathPoint, PolyCenter);
						DrawDebugSphere(GetWorld(), PolyCenter, 20.0, 4, DebugColor, false, 1.0, 0, 10.0);
					}
				});
				PathCorridorUpdatedHandles.Add(Patroller, OnPathCorridorUpdatedHandle);
			}
		}
		else
		{
			for (const auto& PatrollerAndHandle : PathCorridorUpdatedHandles)
			{
				PatrollerAndHandle.Key->OnPathCorridorUpdated().Remove(PatrollerAndHandle.Value);
			}
		}
	}
	
	if (ImGui::Checkbox("Show Waypoint Staleness", &bShowWaypointStaleness))
	{
		if (bShowWaypointStaleness)
		{
			USharedSearchSpaceSubsystem* SharedPatrolWaypointsSubsystem =
				GetWorldRef().GetSubsystem<USharedSearchSpaceSubsystem>();
			const FSearchSpaceInfo* SearchSpaceInfo =
				SharedPatrolWaypointsSubsystem->GetSearchSpaceInfo(*RecastNavMeshes[CurrentSelection]);
			
			for (const auto& SegmentProbability : SearchSpaceInfo->NavNodes)
			{
				FWaypointSegmentDebugInformation SegmentDebugInformation;
				SegmentDebugInformation.SegmentProceduralMesh = NewObject<UProceduralMeshComponent>(SegmentInformationComponentHolder);
				SegmentInformationComponentHolder->AddInstanceComponent(SegmentDebugInformation.SegmentProceduralMesh);
				SegmentDebugInformation.SegmentProceduralMesh->RegisterComponent();
					
				TArray<FVector> PolyVerts;
				RecastNavMeshes[CurrentSelection]->GetPolyVerts(SegmentProbability.Key, PolyVerts);
				FVector Center;
				RecastNavMeshes[CurrentSelection]->GetPolyCenter(SegmentProbability.Key, Center);
				TArray Vertices = {Center, PolyVerts[0]};
				// Close the "loop" of triangles.
				TArray Triangles = {0, PolyVerts.Num(), 1};
				
				for (int i = 1; i < PolyVerts.Num(); i++)
				{
					Vertices.Add(PolyVerts[i]);
						
					Triangles.Add(0);
					Triangles.Add(i);
					// Intentional - The verticies array is always prepended with the center
					Triangles.Add(i + 1);
				}

				SegmentDebugInformation.DynamicBasicShapeMaterialInstance = UMaterialInstanceDynamic::Create(BasicShapeMaterial, this);
				const FLinearColor Color(FMath::Sqrt(SegmentProbability.Value.Probability / 100000000.0), 1 - (FMath::Sqrt(SegmentProbability.Value.Probability / 100000000.0)), 0.0, 1.0);
				SegmentDebugInformation.DynamicBasicShapeMaterialInstance->SetVectorParameterValue(FName(TEXT("Color")), Color);
				SegmentDebugInformation.SegmentProceduralMesh->SetMaterial(0, SegmentDebugInformation.DynamicBasicShapeMaterialInstance);
				SegmentDebugInformation.SegmentProceduralMesh->CreateMeshSection(0, Vertices, Triangles, {}, {}, {}, {}, false);

				SegmentDebugInformation.ProbabilityTextWidget = NewObject<UWidgetComponent>(SegmentInformationComponentHolder);
				SegmentInformationComponentHolder->AddInstanceComponent(SegmentDebugInformation.ProbabilityTextWidget);
				SegmentDebugInformation.ProbabilityTextWidget->RegisterComponent();
				SegmentDebugInformation.ProbabilityTextWidget->SetWorldLocation(Center);
				
				
				SegmentDebugInformation.ProbabilityTextWidget->SetWidgetClass(FloatingTextWidgetType);
				SegmentDebugInformation.ProbabilityTextWidget->InitWidget();
				SegmentDebugInformation.ProbabilityTextWidget->SetWidgetSpace(EWidgetSpace::Screen);
				
				SegmentDebugInformation.ProbabilityText = Cast<UFloatingTextWidget>(SegmentDebugInformation.ProbabilityTextWidget->GetWidget())->ProbabilityText;
				SegmentDebugInformation.ProbabilityText->SetText(FText::AsNumber(SegmentProbability.Value.Probability));
				
				CurrentSegmentInformation.Add(SegmentProbability.Key, SegmentDebugInformation);
			}
			
			SharedPatrolWaypointsSubsystem->OnSegmentProbabilitiesUpdated().AddWeakLambda(this, [this, SharedPatrolWaypointsSubsystem]()
			{
				const FSearchSpaceInfo* SearchSpaceInfo =
					SharedPatrolWaypointsSubsystem->GetSearchSpaceInfo(*RecastNavMeshes[CurrentSelection]);
				for (const auto& SegmentProbability : SearchSpaceInfo->NavNodes)
				{
					FWaypointSegmentDebugInformation* SegmentDebugInformation = CurrentSegmentInformation.Find(SegmentProbability.Key);
					const FLinearColor Color(FMath::Sqrt(SegmentProbability.Value.Probability / 10000.0), 1 - (FMath::Sqrt(SegmentProbability.Value.Probability / 10000.0)), 0.0, 1.0);
					SegmentDebugInformation->DynamicBasicShapeMaterialInstance->SetVectorParameterValue(FName(TEXT("Color")), Color);
					SegmentDebugInformation->ProbabilityText->SetText(FText::AsNumber(SegmentProbability.Value.Probability));
				}
			});
		}
	}

	if (ImGui::Button("Trigger Search Behavior"))
	{
		USharedSearchSpaceSubsystem* SharedPatrolWaypointsSubsystem =
			GetWorldRef().GetSubsystem<USharedSearchSpaceSubsystem>();
		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		SharedPatrolWaypointsSubsystem->SetFocusedSegment(*RecastNavMeshes[CurrentSelection], Character->GetActorLocation());
	}

	if (ImGui::Button("Emit Soft Noise from Player"))
	{
		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		const UDebugDashboardConstants* Constants = GetDefault<UDebugDashboardConstants>();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.ObjectFlags = EObjectFlags::RF_Transient;
		/*
		ASuspiciousSoundEmitter* SuspiciousSoundEmitter =
			GetWorldRef().SpawnActor<ASuspiciousSoundEmitter>(Constants->SuspiciousSoundEmitterType, Character->GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
		TArray<class UAISense_Hearing*> AIListeners;

		UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(GetWorld());
		SuspiciousSoundEmitter->Emit(*PerceptionSystem, 100000.0);
		*/
	}

	if (ImGui::Button("Emit Loud Noise from Player"))
	{
		
	}
	
	if (ImGui::Button("Trigger Attack Behavior"))
	{
		
	}
}
