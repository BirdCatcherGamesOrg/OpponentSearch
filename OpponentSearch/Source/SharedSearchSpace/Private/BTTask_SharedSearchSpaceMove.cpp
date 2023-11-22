// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "BTTask_SharedSearchSpaceMove.h"
#include "AIController.h"
#include "CliqueHandler.h"
#include "ConsoleVariableUtilities.h"
#include "SharedSearchSpaceBlackboardUtils.h"
#include "SharedSearchSpaceSubsystem.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavMesh/NavMeshPath.h"
#include "SharedSearchSpaceAgent.h"
#include "SearchSpaceLogicNavigationQueryFilter.h"
#include "SharedSearchSpaceLogCategory.h"
#include "XLog.h"
#include "BehaviorTree/Decorators/BTDecorator_CompareBBEntries.h"

TAutoConsoleVariable<bool> UBTTask_SharedSearchSpaceMove::CVarShowDebugNavigationPath(
	AUTO_CVAR_NAME("ShowDebugNavigationPath"),
	false,
	TEXT("Show the debug navigation path."));

UBTTask_SharedSearchSpaceMove::UBTTask_SharedSearchSpaceMove()
{
	FilterClass = USearchSpaceLogicNavigationQueryFilter::StaticClass();
}

void UBTTask_SharedSearchSpaceMove::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	const UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		SharedSearchSpaceBlackboardUtils::CliqueHandlerKey.MakeBlackboardKeySelector(*BBAsset, *this, GET_MEMBER_NAME_CHECKED(UBTTask_SharedSearchSpaceMove, CliqueHandlerKey), CliqueHandlerKey);
		SharedSearchSpaceBlackboardUtils::SharedSearchSpaceAgentKey.MakeBlackboardKeySelector(*BBAsset, *this, GET_MEMBER_NAME_CHECKED(UBTTask_SharedSearchSpaceMove, PatrollerSightBoxKey), PatrollerSightBoxKey);
		SharedSearchSpaceBlackboardUtils::MoveToLocation.MakeBlackboardKeySelector(*BBAsset, *this, GET_MEMBER_NAME_CHECKED(UBTTask_SharedSearchSpaceMove, MoveToLocation), MoveToLocation);
		SharedSearchSpaceBlackboardUtils::MoveToInitialAngleKey.MakeBlackboardKeySelector(*BBAsset, *this, GET_MEMBER_NAME_CHECKED(UBTTask_SharedSearchSpaceMove, MoveToInitialAngleKey), MoveToInitialAngleKey);
	}
}

EBTNodeResult::Type UBTTask_SharedSearchSpaceMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result != EBTNodeResult::Succeeded && Result != EBTNodeResult::InProgress)
	{
		return Result;
	}
	
	const UCliqueHandler* CliqueHandler = SharedSearchSpaceBlackboardUtils::CliqueHandlerKey.GetValue(*OwnerComp.GetBlackboardComponent(), CliqueHandlerKey);
	const ARecastNavMesh* RecastNavMesh = CliqueHandler->GetHandle().CliqueIdentifier;
	const FNavPathSharedPtr NavPath = OwnerComp.GetAIOwner()->GetPathFollowingComponent()->GetPath();
	if (NavPath == nullptr)
	{
		return Result;
	}
	
	const FNavMeshPath* NavMeshPath = NavPath->CastPath<FNavMeshPath>();
	USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem =
		UWorld::GetSubsystem<USharedSearchSpaceSubsystem>(OwnerComp.GetWorld());

	USharedSearchSpaceAgent* PatrollerSightBox =
		SharedSearchSpaceBlackboardUtils::SharedSearchSpaceAgentKey.GetValue(*OwnerComp.GetBlackboardComponent(), PatrollerSightBoxKey);
	
	const FPathCrossedSignature::FDelegate OnPathCrossed =
		FPathCrossedSignature::FDelegate::CreateWeakLambda(PatrollerSightBox, [this, PatrollerSightBox, OwnerComp = &OwnerComp, NavPath, RecastNavMesh](const USharedSearchSpaceAgent& SeenBy, const NavNodeRef& NavNode)
	{
		if (PatrollerSightBox != &SeenBy)
		{
			const FNavMeshPath* NavMeshPath = NavPath->CastPath<FNavMeshPath>();
			
			uint32 CurrentPathIndex = 0;
			// check if is closer to first or second path point (don't assume AI's standing)
			const FVector CurrentLocation = PatrollerSightBox->GetComponentLocation();
			for (int32 PathPoint = 0; PathPoint < NavPath->GetPathPoints().Num() - 1; ++PathPoint)
			{
				const FVector PathPt0 = *NavPath->GetPathPointLocation(PathPoint);
				const FVector PathPt1 = *NavPath->GetPathPointLocation(PathPoint + 1);
				// making this test in 2d to avoid situation where agent's Z location not being in "navmesh plane"
				// would influence the result
				const FVector::FReal SqDistToFirstPoint = (CurrentLocation - PathPt0).SizeSquared2D();
				const FVector::FReal SqDistToSecondPoint = (CurrentLocation - PathPt1).SizeSquared2D();

				CurrentPathIndex = FMath::IsNearlyEqual(SqDistToFirstPoint, SqDistToSecondPoint) ?
					((FMath::Abs(CurrentLocation.Z - PathPt0.Z) < FMath::Abs(CurrentLocation.Z - PathPt1.Z)) ? PathPoint : CurrentPathIndex) :
					((SqDistToFirstPoint < SqDistToSecondPoint) ? CurrentPathIndex : PathPoint);
			}

			const uint32 SeenPathIndex = NavMeshPath->GetNodeRefIndex(NavNode);
			
			if (CurrentPathIndex == 0)
			{
				XLOG(LogSharedSearchSpace, Log, "Stutter step skip, still on path index 0.",
					 {{"currentPathIndex", FString::FromInt(CurrentPathIndex)}});
				return;
			}

			if (SeenPathIndex < CurrentPathIndex)
			{
				XLOG(LogSharedSearchSpace, Log, "Saw node that's behind current path index",
					{{"seenPathPoint", FString::FromInt(SeenPathIndex)},
					 {"currentPathIndex", FString::FromInt(CurrentPathIndex)}});
				return;
			}

			XLOG(LogSharedSearchSpace, Warning, "Paths crossed, node index info",
				{{"seenPathPoint", FString::FromInt(SeenPathIndex)},
				 {"currentPathIndex", FString::FromInt(CurrentPathIndex)}});
			USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem =
				UWorld::GetSubsystem<USharedSearchSpaceSubsystem>(OwnerComp->GetWorld());
			SharedSearchSpaceSubsystem->UnsetNodesOnPaths(*PatrollerSightBox, *RecastNavMesh, NavMeshPath->PathCorridor);
			// Test: Setting -1. Does this skip the "stutter steps" behavior for overlapping sight boxes?
			SharedSearchSpaceBlackboardUtils::MoveToInitialAngleKey.SetValue(*OwnerComp->GetBlackboardComponent(), MoveToInitialAngleKey, -1);
			OwnerComp->GetAIOwner()->GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::OwnerFinished);
		}
	});

	const bool bSetNodes = SharedSearchSpaceSubsystem->SetNodesOnPaths(*PatrollerSightBox, *RecastNavMesh, NavMeshPath->PathCorridor, OnPathCrossed);

	if (!bSetNodes)
	{
		OwnerComp.GetAIOwner()->GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::OwnerFinished);
		return Result;
	}

	FTSTicker::FDelegateHandle ShowDebugHandle;
	if (CVarShowDebugNavigationPath.GetValueOnGameThread())
	{
		TArray<FVector> NavPoints;
		Algo::Transform(NavMeshPath->PathCorridor, NavPoints, [RecastNavMesh](const NavNodeRef& NavNodeRef)
		{
			FVector Point;
			RecastNavMesh->GetPolyCenter(NavNodeRef, Point);
			return Point;
		});
		UWorld* WorldContext = OwnerComp.GetAIOwner()->GetWorld();
		const FTickerDelegate ShowDebugTicker = FTickerDelegate::CreateWeakLambda(this, [this, WorldContext, NavPoints](float Delay)
		{
			for (int i = 0; i < NavPoints.Num() - 1; ++i)
			{
				DrawDebugLine(
					WorldContext,
					NavPoints[i],
					NavPoints[i+1],
					FColor::Red,
					false,
					0,
					0,
					1
				);
			}
			return true;
		});
		ShowDebugHandle = FTSTicker::GetCoreTicker().AddTicker(ShowDebugTicker);
	}
	
	if (!OwnerComp.GetAIOwner()->GetPathFollowingComponent()->OnRequestFinished.IsBoundToObject(this))
	{
		OwnerComp.GetAIOwner()->GetPathFollowingComponent()->OnRequestFinished.AddWeakLambda(this, [this, ShowDebugHandle, OwnerComp = &OwnerComp, NavPath, RecastNavMesh, PatrollerSightBox](FAIRequestID /*RequestID*/, const FPathFollowingResult& /*Result*/)
		{
			FTSTicker::GetCoreTicker().RemoveTicker(ShowDebugHandle);
			const FNavMeshPath* NavMeshPath = NavPath->CastPath<FNavMeshPath>();
			USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem =
				UWorld::GetSubsystem<USharedSearchSpaceSubsystem>(OwnerComp->GetWorld());
			SharedSearchSpaceSubsystem->UnsetNodesOnPaths(*PatrollerSightBox, *RecastNavMesh, NavMeshPath->PathCorridor);
			// SharedSearchSpaceBlackboardUtils::MoveToInitialAngleKey.SetValue(*OwnerComp->GetBlackboardComponent(), MoveToInitialAngleKey, -1);
			OwnerComp->GetAIOwner()->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		});
	}
	
	const float MoveToInitialAngle = PatrollerSightBox->GetOwner()->GetActorForwardVector().Dot(NavPath->GetSegmentDirection(0));
	SharedSearchSpaceBlackboardUtils::MoveToInitialAngleKey.SetValue(*OwnerComp.GetBlackboardComponent(), MoveToInitialAngleKey, MoveToInitialAngle); 
	return Result;
}
