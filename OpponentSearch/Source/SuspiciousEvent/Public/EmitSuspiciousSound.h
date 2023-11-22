// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EmitSuspiciousSound.generated.h"

UCLASS()
class UEmitSuspiciousSoundPayload : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<class UAudioComponent> AudioComponent;

	UPROPERTY()
	TArray<FAudioParameter> AudioParameters;

	UPROPERTY()
	float Loudness;
	
	UPROPERTY()
	TObjectPtr<class UAIPerceptionSystem> AIListeners;
	
};

/**
 * 
 */
UCLASS()
class SUSPICIOUSEVENT_API UEmitSuspiciousSound : public UGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
