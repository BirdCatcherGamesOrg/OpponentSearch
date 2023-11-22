// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.


#include "EmitSuspiciousSound.h"

#include "SuspiciousEventSettings.h"
#include "Components/AudioComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Hearing.h"

void UEmitSuspiciousSound::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	UEmitSuspiciousSoundPayload* Payload = Cast<UEmitSuspiciousSoundPayload>(TriggerEventData->OptionalObject);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	TArray<FAudioParameter> AudioParameters;
	AudioParameters.Append(Payload->AudioParameters);
	Payload->AudioComponent->SetParameters(MoveTemp(AudioParameters));
	FAINoiseEvent Event;
	Event.NoiseLocation = Payload->AudioComponent->GetComponentLocation();
	Event.Loudness = Payload->Loudness;
	AActor* Avatar = ActorInfo->AvatarActor.Get();
	Event.Instigator = Avatar;
	const USuspiciousEventSettings* SuspiciousEventSettings = GetDefault<USuspiciousEventSettings>();
	Event.Tag = SuspiciousEventSettings->SuspiciousStimulusTag;
	Payload->AIListeners->OnEvent(Event);
}
