// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "SuspiciousSoundEmitter.generated.h"

UCLASS()
class SUSPICIOUSEVENT_API ASuspiciousSoundEmitter : public AActor
{
	GENERATED_BODY()

public:

	ASuspiciousSoundEmitter();

	virtual void PostInitializeComponents() override;
	
	void Emit(class UAIPerceptionSystem& AIListeners, float Loudness);

private:

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UAudioComponent> SoundEmitter;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TArray<FAudioParameter> AudioParameters;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TSubclassOf<class UGameplayAbility> EmitSoundAbilityType;

	UPROPERTY()
	TObjectPtr<class UEmitSuspiciousSoundPayload> Payload;
	
	UPROPERTY()
	FGameplayEventData GameplayEventData;

	UPROPERTY()
	FGameplayAbilitySpecHandle EmitSuspiciousSoundHandle;
};
