// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OpponentSearchPlayerController.generated.h"

/**
 * @brief Character for the demo OpponentSearch project.
 * Can move and uses basic camera controls, but lacks any other gameplay features.
 */
UCLASS(Abstract, Blueprintable)
class CHARACTER_API AOpponentSearchPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	AOpponentSearchPlayerController();

	/**
	 * @copydoc APlayerController::OnPossess
	 */
	virtual void OnPossess(APawn* PossessedPawn) override;

private:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UMovementInputComponent> MovementInputComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TSubclassOf<class UUserWidget> TopLevelWidgetClass;

};
