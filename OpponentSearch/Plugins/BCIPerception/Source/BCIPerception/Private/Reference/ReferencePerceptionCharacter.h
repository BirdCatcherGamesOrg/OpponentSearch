// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISightTargetInterface.h"
#include "ReferencePerceptionCharacter.generated.h"

UCLASS()
class BCIPERCEPTION_API AReferencePerceptionCharacter : public ACharacter, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AReferencePerceptionCharacter();
	
	virtual void PostInitializeComponents() override;

	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate) override;
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCVarBasedAISightTarget> CVarBasedAISightTarget;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UTaggedSightTargetComponent> TaggedSightTarget;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class USenseConfigPublisher> SenseConfigPublisher;
	
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Hearing> NativeHearing;

	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> NativeVision;
	
};
