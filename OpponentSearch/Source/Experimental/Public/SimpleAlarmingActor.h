// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "SimpleAlarmingActor.generated.h"

UCLASS()
class EXPERIMENTALMODULE_API ASimpleAlarmingActor : public ACharacter, public IAISightTargetInterface
{
	GENERATED_BODY()
	
public:

	ASimpleAlarmingActor();

	virtual void BeginPlay() override;
	
	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate) override;
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCVarBasedAISightTarget> CVarBasedAISightTarget;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UTaggedSightTargetComponent> TaggedSightTarget;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class USphereComponent> AcceptableRadiusBounds;
	
};
