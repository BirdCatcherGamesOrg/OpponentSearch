// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "AlarmStimulusBehaviorComponent.h"

#include "AIController.h"
#include "AlarmStimulusCliqueTrackerSubsystem.h"
#include "CliqueHandler.h"
#include "ExperimentalModuleLogCategory.h"
#include "ExperimentalModuleSettings.h"
#include "PatrolSharedSearchSpaceComponent.h"
#include "SenseConfigPublisher.h"
#include "XLog.h"

UAlarmStimulusBehaviorComponent::UAlarmStimulusBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UAlarmStimulusBehaviorComponent::SetDependencies(const UCliqueHandler& InClique, USenseConfigPublisher& SenseConfigPublisher, AAIController& InAIController, UPatrolSharedSearchSpaceComponent& InPatrolSharedSearchSpaceComponent, UNativeSharedSearchSpaceLogicComponent& InNativeSharedSearchSpaceLogicComponent)
{
	Clique = &InClique;
	AIController = &InAIController;
	PatrolSharedSearchSpaceComponent = &InPatrolSharedSearchSpaceComponent;
	NativeSharedSearchSpaceLogicComponent = &InNativeSharedSearchSpaceLogicComponent;
	UAlarmStimulusCliqueTrackerSubsystem* AlarmStimulusCliqueTrackerSubsystem =
		UWorld::GetSubsystem<UAlarmStimulusCliqueTrackerSubsystem>(GetWorld());
	AlarmStimulusCliqueTrackerSubsystem->RegisterAlarmStimulusBehavior(InClique.GetHandle(), *this);

	const UExperimentalModuleSettings* Settings = GetDefault<UExperimentalModuleSettings>();
	SenseConfigPublisher.OnSensed(Settings->AlarmingStimulusTag).AddWeakLambda(this, [this](const FActorPerceptionUpdateInfo& Info)
	{
		XLOG(LogExperimentalModule, Verbose, "Sensed alarming stimulus",
			{{"self", GetOwner()->GetName()}});

		// This method is non-reentrant if we're already reacting to a stimulus on delay
		if (StimulusReactingDelayHandle.IsValid())
		{
			XLOG(LogExperimentalModule, Verbose, "Stimulus raction is playing, don't interrupt");
			return;
		}
		
		if (Info.Stimulus.WasSuccessfullySensed())
		{
			XLOG(LogExperimentalModule, Verbose, "Begin successfully sensed branch");
			
			// Interrupt the stimulus being lost, if needed.
			if (DelayedLostStimulusHandle.IsValid())
			{
				FTSTicker::GetCoreTicker().RemoveTicker(DelayedLostStimulusHandle);
				return;
			}

			// Rest of this function is non-reentrant if the actor is being pursued.
			// Otherwise, alarm reactors would each react with their widget animations, etc.
			if (ActorToPursue != nullptr)
			{
				XLOG(LogExperimentalModule, Verbose, "Already pursuing actor.",
					{{"self", GetOwner()->GetName()},
					 {"actor", ActorToPursue->GetName()},
					 {"location", ActorToPursue->GetActorLocation().ToString()}});
				UAlarmStimulusCliqueTrackerSubsystem* AlarmStimulusCliqueTrackerSubsystem =
					UWorld::GetSubsystem<UAlarmStimulusCliqueTrackerSubsystem>(GetWorld());
				AlarmStimulusCliqueTrackerSubsystem->RegisterAlarmingLocation(Clique->GetHandle(), *this, *Info.Target.Get());
				return;
			}

			// Stop patrolling, display the alarm status widget, then signal to the clique that a target is available.
			PatrolSharedSearchSpaceComponent->StopPatrolling();
			
			// TODO: More like doing an entire animation, etc?
			UObject* StatusWidgetModel = nullptr;// AIController->GetPawn()->FindComponentByClass<UStatusWidgetModel>();
			if (StatusWidgetModel != nullptr)
			{
				//StimulusReactingDelayHandle = StatusWidgetModel->SetStatus(SuspiciousEventSettings->TemporaryReactingToAlarmEventWidgetStatus, StatusUpdatedSignature);
			}
			else
			{
				UAlarmStimulusCliqueTrackerSubsystem* AlarmStimulusCliqueTrackerSubsystem =
					UWorld::GetSubsystem<UAlarmStimulusCliqueTrackerSubsystem>(GetWorld());
				AlarmStimulusCliqueTrackerSubsystem->RegisterAlarmingLocation(Clique->GetHandle(), *this, *Info.Target.Get());
			}
		}
		else
		{
			// Not sure if it's possible (Lost sight twice?), but this is also non-reentrant.
			if (DelayedLostStimulusHandle.IsValid())
			{
				return;
			}
			
			XLOG(LogExperimentalModule, Verbose, "Lost sight. Start waiting for slipping to the search state.",
				{{"self", GetOwner()->GetName()}});
			FVector LastKnownAlarmActorLocation = Info.Target->GetActorLocation();
			const FTickerDelegate DelayedLostStimulus = FTickerDelegate::CreateWeakLambda(this, [this, Info, LastKnownAlarmActorLocation](float Delay)
			{
				XLOG(LogExperimentalModule, Verbose, "Slip to searching state.",
					{{"self", GetOwner()->GetName()}});
				UAlarmStimulusCliqueTrackerSubsystem* AlarmStimulusCliqueTrackerSubsystem =
					UWorld::GetSubsystem<UAlarmStimulusCliqueTrackerSubsystem>(GetWorld());
				AlarmStimulusCliqueTrackerSubsystem->RemoveAlarmingLocation(*Clique, *this, LastKnownAlarmActorLocation);
				return false;
			});
			DelayedLostStimulusHandle = FTSTicker::GetCoreTicker().AddTicker(DelayedLostStimulus, TimeToDelayLostStimulus);
		}
	});
}

const AActor* UAlarmStimulusBehaviorComponent::GetAlarmStimulusActor()
{
	return ActorToPursue;
}

void UAlarmStimulusBehaviorComponent::SetAlarmStimulusActor(AActor& Actor)
{
	XLOG(LogExperimentalModule, Verbose, "Move to alarm actor",
		{{"self", GetOwner()->GetName()},
		 {"actor", Actor.GetName()},
		 {"location", Actor.GetActorLocation().ToString()}});
	PatrolSharedSearchSpaceComponent->StopPatrolling();
	ActorToPursue = &Actor;
}

void UAlarmStimulusBehaviorComponent::EndAlarm()
{
	XLOG(LogExperimentalModule, Verbose, "Ending alarm",
		{{"self", GetOwner()->GetName()}});
	ActorToPursue = nullptr;
}

FVector UAlarmStimulusBehaviorComponent::GetNavAgentLocation() const
{
	return AIController->GetNavAgentLocation();
}
