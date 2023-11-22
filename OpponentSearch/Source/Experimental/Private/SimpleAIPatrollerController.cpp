// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.


#include "SimpleAIPatrollerController.h"

#include "AlarmStimulusBehaviorComponent.h"
#include "AlarmStimulusCliqueTrackerSubsystem.h"
#include "CliqueHandler.h"
#include "ExperimentalModuleBlackboardUtils.h"
#include "ExperimentalModuleLogCategory.h"
#include "ExperimentalModuleSettings.h"
#include "SenseConfigPublisher.h"
#include "SharedSearchSpaceAgent.h"
#include "SharedSearchSpaceBlackboardUtils.h"
#include "SharedSearchSpaceSubsystem.h"
#include "XLog.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

ASimpleAIPatrollerController::ASimpleAIPatrollerController()
{
	PrimaryActorTick.bCanEverTick = false;
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BrainComponent = BehaviorTreeComponent;
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void ASimpleAIPatrollerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Blackboard->InitializeBlackboard(*BlackboardData);
}

void ASimpleAIPatrollerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// AI stuff.
	UCliqueHandler* CliqueHandler = InPawn->FindComponentByClass<UCliqueHandler>();
	SharedSearchSpaceBlackboardUtils::CliqueHandlerKey.SetValue(*Blackboard, CliqueHandler);
	USharedSearchSpaceAgent* SharedSearchSpaceAgent = InPawn->FindComponentByClass<USharedSearchSpaceAgent>();
	SharedSearchSpaceBlackboardUtils::SharedSearchSpaceAgentKey.SetValue(*Blackboard, SharedSearchSpaceAgent);

	// Patroller stuff
	USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem = GetWorld()->GetSubsystem<USharedSearchSpaceSubsystem>();
	SharedSearchSpaceSubsystem->CreateSearchSpaces(*SharedSearchSpaceAgent, *CliqueHandler->GetHandle().CliqueIdentifier);

	// Alarm Stimulus
	/*
	UAlarmStimulusCliqueTrackerSubsystem* CliqueTrackerSubsystem =
		UWorld::GetSubsystem<UAlarmStimulusCliqueTrackerSubsystem>(GetWorld());

	USenseConfigPublisher* SenseConfigPublisher = InPawn->FindComponentByClass<USenseConfigPublisher>();
	SenseConfigPublisher->SetActivePerception(*PerceptionComponent);
	
	const UExperimentalModuleSettings* Settings = GetDefault<UExperimentalModuleSettings>();
	SenseConfigPublisher->OnSensed(Settings->AlarmingStimulusTag).AddWeakLambda(this, [this, CliqueHandler, CliqueTrackerSubsystem](const FActorPerceptionUpdateInfo& Info)
	{
		if (Info.Stimulus.WasSuccessfullySensed())
		{
			CliqueTrackerSubsystem->RegisterAlarmingLocation(CliqueHandler->GetHandle(), , Info.Target);
		}
		else if (!Info.Stimulus.WasSuccessfullySensed())
		{
			CliqueTrackerSubsystem->RemoveAlarmingLocation(CliqueHandler->GetHandle(), , Info.)
		}
		{
			
		}
	});
	CliqueTrackerSubsystem->OnCliqueEnteredAlarm().AddWeakLambda(this, [this]()
	{
		XLOG(LogExperimentalModule, Verbose, "Alarm stimulus detected",
			{{"location", AlarmStimulusBehaviorComponent->GetAlarmStimulusActor()->GetActorLocation().ToString()}});
		
		AlarmStimulusLocationAccessor.SetValue(*Blackboard, AlarmStimulusBehaviorComponent->GetAlarmStimulusActor()->GetActorLocation());
	});

	CliqueTrackerSubsystem->OnCliqueExitedAlarm().AddWeakLambda(this, [this]()
	{
		XLOG(LogExperimentalModule, Verbose, "Alarm stimulus detected",
			{{"location", AlarmStimulusBehaviorComponent->GetAlarmStimulusActor()->GetActorLocation().ToString()}});
		ExperimentalModuleBlackboardUtils::AlarmStimulusLocationKey.ClearValue(*Blackboard);
	});
	*/
	BrainComponent->StartLogic();
}
