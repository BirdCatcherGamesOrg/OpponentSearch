// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "SenseConfigPublisher.h"
#include "PerceptionLogCategory.h"
#include "TaggedSightTargetComponent.h"
#include "XLog.h"
#include "Perception/AISense_Sight.h"

USenseConfigPublisher::USenseConfigPublisher()
{
	PrimaryComponentTick.bCanEverTick = false;
}

const TArray<UAISenseConfig*>& USenseConfigPublisher::GetSenses()
{
	return AvailableSenses;
}

int32 USenseConfigPublisher::SetActivePerception(UAIPerceptionComponent& InActivePerception)
{
	if (ActivePerception != nullptr)
	{
		ActivePerception->OnTargetPerceptionInfoUpdated.RemoveDynamic(this, &USenseConfigPublisher::OnTargetPerceptionInfoUpdated);
	}

	int32 SensesConfigured = 0;
	
	UAIPerceptionSystem* AIPerceptionSys = UAIPerceptionSystem::GetCurrent(GetWorld());
	// Required to initialize the perception component with the perception system.
	// Normally, this isn't needed because the perception component will have senses on it already.
	// However since this approach pulls the senses from an external source (usually the pawn!),
	// this isn't called because the AI system won't automatically initialize it as an optimization.
	// An explicit call is required.
	AIPerceptionSys->UpdateListener(InActivePerception);
	// Add the senses
	for (UAISenseConfig* Sense : AvailableSenses)
	{
		if (Sense != nullptr)
		{
			SensesConfigured++;
			InActivePerception.ConfigureSense(*Sense);
		}
	}
	// Required to update the perception system to pick up the newly configured senses on the perception component.
	AIPerceptionSys->UpdateListener(InActivePerception);
	ActivePerception = &InActivePerception;
	ActivePerception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &USenseConfigPublisher::OnTargetPerceptionInfoUpdated);
	return SensesConfigured;
}

void USenseConfigPublisher::OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	FName StimulusTag = UpdateInfo.Stimulus.Tag;
	if (UpdateInfo.Stimulus.Type == GetDefault<UAISense_Sight>()->GetSenseID() && StimulusTag.IsNone() && UpdateInfo.Target != nullptr)
	{
		XLOG(LogPerception, Verbose, "Sensed sight stimulus without a tag.");
		const UTaggedSightTargetComponent* TaggedSightTargetComponent = UpdateInfo.Target->FindComponentByClass<UTaggedSightTargetComponent>();
		if (TaggedSightTargetComponent != nullptr)
		{
			StimulusTag = TaggedSightTargetComponent->GetVisibleStimulusTag();
			XLOG(LogPerception, Verbose, "Stimulus has sight tag component",
				{{"newTag", StimulusTag.ToString()}});
		}
	}
	
	const FSensedStimulusSignature* OnSensed = OnSensedByTag.Find(StimulusTag);
	if (OnSensed == nullptr)
	{
		return;
	}
	OnSensed->Broadcast(UpdateInfo);
}

void USenseConfigPublisher::AddNativeSenses(const TArray<UAISenseConfig*>& NativeSenses)
{
	AvailableSenses.Append(NativeSenses);
}

FSensedStimulusSignature& USenseConfigPublisher::OnSensed(const FName& StimulusTag)
{
	return OnSensedByTag.FindOrAdd(StimulusTag);
}
