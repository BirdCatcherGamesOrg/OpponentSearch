// Copyright 2021 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "OpponentSearchPlayerCharacter.generated.h"

/**
 * @brief Character for the demo OpponentSearch project.
 * Can move and uses basic camera controls, but lacks any other gameplay features.
 */
UCLASS(Abstract, Blueprintable, Config=Game)
class CHARACTER_API AOpponentSearchPlayerCharacter : public ACharacter, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	AOpponentSearchPlayerCharacter();

	/**
	 * @copydoc IAISightTargetInterface::CanBeSeenFrom
	 */
	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate) override;
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCVarBasedAISightTarget> CVarBasedAISightTarget;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UTaggedSightTargetComponent> TaggedSightTarget;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCharacterRadarComponent> CharacterRadarComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCharacterCamera> CharacterCamera;
	
};

