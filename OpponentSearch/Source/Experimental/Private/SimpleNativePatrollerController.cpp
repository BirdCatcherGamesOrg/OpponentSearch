// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "SimpleNativePatrollerController.h"
#include "AlarmStimulusCliqueTrackerSubsystem.h"
#include "AlarmStimulusBehaviorComponent.h"
#include "CliqueHandler.h"
#include "ExperimentalModuleLogCategory.h"
#include "SharedSearchSpaceAgent.h"
#include "Navigation/PathFollowingComponent.h"
#include "SenseConfigPublisher.h"
#include "SuspiciousEventSettings.h"
#include "Perception/AIPerceptionComponent.h"
#include "PatrolSharedSearchSpaceComponent.h"
#include "NativeSharedSearchSpaceLogicComponent.h"
#include "XLog.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASimpleNativePatrollerController::ASimpleNativePatrollerController()
{
	PrimaryActorTick.bCanEverTick = false;
	PatrolSharedSearchSpaceComponent = CreateDefaultSubobject<UPatrolSharedSearchSpaceComponent>(TEXT("PatrolSharedSearchSpaceComponent"));
	AlarmStimulusBehaviorComponent = CreateDefaultSubobject<UAlarmStimulusBehaviorComponent>(TEXT("AlarmStimulusBehaviorComponent"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	NativeSharedSearchSpaceLogicComponent = CreateDefaultSubobject<UNativeSharedSearchSpaceLogicComponent>(TEXT("SharedSearchSpaceLogicComponent"));
	bSetControlRotationFromPawnOrientation = true;
}

void ASimpleNativePatrollerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASimpleNativePatrollerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	const UCliqueHandler* CliqueHandler = InPawn->FindComponentByClass<UCliqueHandler>();
	if (CliqueHandler == nullptr)
	{
		XLOG(LogExperimentalModule, Warning, "Requires CliqueHandler to function.");
		return;
	}
	
	// Setup senses
	USenseConfigPublisher* SenseConfigPublisher = InPawn->FindComponentByClass<USenseConfigPublisher>();
	if (SenseConfigPublisher == nullptr)
	{
		XLOG(LogExperimentalModule, Warning, "Requires Senses to function.");
		return;
	}
	
	const int32 NumberOfSenses = SenseConfigPublisher->SetActivePerception(*GetPerceptionComponent());
	XLOG(LogExperimentalModule, Verbose, "Configured senses", {{"numberOfSenses", FString::FromInt(NumberOfSenses)}});
	
	// Investigates suspicious sounds
	const USuspiciousEventSettings* SuspiciousEventSettings = GetDefault<USuspiciousEventSettings>();
	SenseConfigPublisher->OnSensed(SuspiciousEventSettings->SuspiciousStimulusTag).AddWeakLambda(this, [this, SuspiciousEventSettings]( const FActorPerceptionUpdateInfo& Info) mutable
	{
		XLOG(LogExperimentalModule, Verbose, "Sensed suspicious stimulus");
		PatrolSharedSearchSpaceComponent->StopPatrolling();

		// If we arrived at the location, stay in place for a set amount of time before returning to patrol
		InvestigateSuspiciousLocationTimeHandle = GetPathFollowingComponent()->OnRequestFinished.AddWeakLambda(this, [this](FAIRequestID RequestID, const FPathFollowingResult& Result)
		{
			XLOG(LogExperimentalModule, Verbose, "At suspicious stimulus location. Waiting");
			const FTickerDelegate DelayedReturnToPatrol = FTickerDelegate::CreateWeakLambda(this, [this](float Delay)
			{
				XLOG(LogExperimentalModule, Verbose, "Returning to patrol");
				PatrolSharedSearchSpaceComponent->StartPatrolling(*this, *NativeSharedSearchSpaceLogicComponent);
				return false;
			});
			FTSTicker::GetCoreTicker().AddTicker(DelayedReturnToPatrol, InvestigateSuspiciousLocationTime);
			GetPathFollowingComponent()->OnRequestFinished.Remove(InvestigateSuspiciousLocationTimeHandle);
		});

		/*
		// Only move once the status update animation has played.
		UStatusWidgetModel::FStatusUpdatedSignature StatusUpdatedSignature;
		StatusUpdatedSignature.AddWeakLambda(this, [this, Info]()
		{
			// Investigate a specific point
			XLOG(LogExperimentalModule, Verbose, "Move to investigate the suspicious stimulus",
				{{"location", Info.Stimulus.StimulusLocation.ToString()}});

			FAIMoveRequest MoveRequest(Info.Stimulus.StimulusLocation);
			MoveTo(MoveRequest);
		});

		// Visually indicate entering an investigation state.
		// TODO: More like doing an entire animation, etc?
		UStatusWidgetModel* StatusWidgetModel = GetPawn()->FindComponentByClass<UStatusWidgetModel>();
		if (StatusWidgetModel != nullptr)
		{
			XLOG(LogExperimentalModule, Verbose, "Updating suspicious stimulus status widget");
			StatusWidgetModel->SetStatus(SuspiciousEventSettings->ReactingToSuspiciousEventWidgetStatus, StatusUpdatedSignature);
		}
		else
		{
			StatusUpdatedSignature.Broadcast();
		}
		*/
	});

	
	// Alarm on events
	AlarmStimulusBehaviorComponent->SetDependencies(
		*CliqueHandler,
		*SenseConfigPublisher, 
		*this,
		*PatrolSharedSearchSpaceComponent,
		*NativeSharedSearchSpaceLogicComponent);
	
	
	USharedSearchSpaceAgent* SharedSearchSpaceAgent = InPawn->FindComponentByClass<USharedSearchSpaceAgent>();
	NativeSharedSearchSpaceLogicComponent->AttachToComponent(SharedSearchSpaceAgent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	NativeSharedSearchSpaceLogicComponent->SetDependencies(
		*this,
		*CliqueHandler->GetHandle().CliqueIdentifier.Get(),
		*SharedSearchSpaceAgent,
		1);

	UAlarmStimulusCliqueTrackerSubsystem* CliqueTrackerSubsystem =
		UWorld::GetSubsystem<UAlarmStimulusCliqueTrackerSubsystem>(GetWorld());

	/*
	CliqueTrackerSubsystem->OnCliqueEnteredAlarm().AddWeakLambda(this, [this]()
	{
		XLOG(LogExperimentalModule, Verbose, "Begin running logic");
		AlarmStimulusLocationAccessor.SetValue(*Blackboard, AlarmStimulusBehaviorComponent->GetAlarmStimulusActor()->GetActorLocation());
		BrainComponent->StartLogic();
	});

*/
	CliqueTrackerSubsystem->OnCliqueExitedAlarm().AddWeakLambda(this, [this]()
	{
		BrainComponent->StopLogic(TEXT(""));
		PatrolSharedSearchSpaceComponent->StartPatrolling(*this, *NativeSharedSearchSpaceLogicComponent);
	});
	
	PatrolSharedSearchSpaceComponent->StartPatrolling(*this, *NativeSharedSearchSpaceLogicComponent);
}

void ASimpleNativePatrollerController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetPawn()->GetActorLocation();
	OutRotation = GetPawn()->GetActorRotation();
}
