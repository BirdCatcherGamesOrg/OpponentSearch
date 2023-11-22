// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "AlarmStimulusCliqueTrackerSubsystem.h"
#include "AlarmStimulusBehaviorComponent.h"
#include "CliqueHandler.h"
#include "ExperimentalModuleLogCategory.h"
#include "SharedSearchSpaceSubsystem.h"
#include "XLog.h"

void UAlarmStimulusCliqueTrackerSubsystem::RegisterAlarmingLocation(const FCliqueHandle& Clique, class UAlarmStimulusBehaviorComponent& AlarmStimulusBehavior, AActor& AlarmStimulusTarget)
{
	FAlarmStimulusCliqueData& CliqueData = AlarmStimulusCliques.FindOrAdd(Clique);
	CliqueData.NumberSensingTarget++;

	XLOG(LogExperimentalModule, Verbose, "Adding alarming location",
		{{"alarmStimulusTarget", AlarmStimulusTarget.GetName()},
		 {"numberSensingTarget", FString::FromInt(CliqueData.NumberSensingTarget)}});
	
	if (CliqueData.AlarmStimulusTarget != nullptr)
	{
		return;
	}

	//OnCliqueEnteredAlarmSignature.Broadcast();
}

void UAlarmStimulusCliqueTrackerSubsystem::RemoveAlarmingLocation(const UCliqueHandler& Clique, UAlarmStimulusBehaviorComponent& AlarmStimulusBehavior, const FVector& LastKnownAlarmStimulusTargetLocation)
{
	FAlarmStimulusCliqueData& CliqueData = AlarmStimulusCliques.FindOrAdd(Clique.GetHandle());
	CliqueData.NumberSensingTarget--;

	XLOG(LogExperimentalModule, Verbose, "Removing alarming location",
		{{"behavior", AlarmStimulusBehavior.GetOwner()->GetName()},
		 {"numberSensingTarget", FString::FromInt(CliqueData.NumberSensingTarget)}});
	
	// When all clique members have lost track of the last location of the player, remove it from all members.
	if (CliqueData.NumberSensingTarget != 0)
	{
		return;
	}

	CliqueData.AlarmStimulusTarget = nullptr;
	USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem = UWorld::GetSubsystem<USharedSearchSpaceSubsystem>(GetWorld());
	const bool bWasLastKnownFocused = SharedSearchSpaceSubsystem->SetFocusedSegment(*Clique.GetHandle().CliqueIdentifier, LastKnownAlarmStimulusTargetLocation);
	if (!bWasLastKnownFocused)
	{
		SharedSearchSpaceSubsystem->SetFocusedSegment(*Clique.GetHandle().CliqueIdentifier, AlarmStimulusBehavior.GetNavAgentLocation());
	}
	
	for (UAlarmStimulusBehaviorComponent* AlarmStimulusBehaviorComponent : CliqueData.CliqueMembers)
	{
		AlarmStimulusBehaviorComponent->EndAlarm();
	}
	OnCliqueExitedAlarmSignature.Broadcast();
}

void UAlarmStimulusCliqueTrackerSubsystem::RegisterAlarmStimulusBehavior(const FCliqueHandle& Clique, UAlarmStimulusBehaviorComponent& AlarmStimulusBehavior)
{
	FAlarmStimulusCliqueData& CliqueData = AlarmStimulusCliques.FindOrAdd(Clique);
	CliqueData.CliqueMembers.Add(&AlarmStimulusBehavior);
}

