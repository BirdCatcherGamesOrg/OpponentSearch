// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterWithBoundableCamera.generated.h"

UCLASS()
class BCICINEMATICCAMERA_API ACharacterWithBoundableCamera : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterWithBoundableCamera();

private:

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	TObjectPtr<class UCharacterCamera> CharacterCamera;
	
};
