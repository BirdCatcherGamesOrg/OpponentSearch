// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterWithBoundableCameraController.generated.h"

/**
 * 
 */
UCLASS()
class BCICINEMATICCAMERA_API ACharacterWithBoundableCameraController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void OnPossess(APawn* InPawn) override;
	
};
