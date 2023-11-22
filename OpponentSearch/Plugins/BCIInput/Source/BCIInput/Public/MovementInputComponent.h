// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MovementInputComponent.generated.h"

/**
 * @brief A reusable component for 1st person or 3rd person character movement.
 */
UCLASS()
class BCIINPUT_API UMovementInputComponent : public UInputComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Set the AController instance to move.
	 * @param InController Controller to use.
	 */
	void SetPlayerController(class AController& InController);

private:

	UPROPERTY()
	TObjectPtr<class AController> Controller;
	
};
