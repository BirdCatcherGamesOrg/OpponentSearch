// Copyright 2021 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BCIInputReferenceController.generated.h"

/**
 * 
 */
UCLASS()
class BCIINPUT_API ABCIInputReferenceController : public APlayerController
{
	GENERATED_BODY()

public:

	ABCIInputReferenceController();
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UMovementInputComponent> MovementInputComponent;

	virtual void OnPossess(APawn* PossessedPawn) override;
	
};
