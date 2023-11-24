// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.


#include "CharacterWithBoundableCameraController.h"

#include "CharacterCamera.h"
#include "Camera/CameraActor.h"

void ACharacterWithBoundableCameraController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	const UCharacterCamera* CharacterCamera = InPawn->FindComponentByClass<UCharacterCamera>();
	if (CharacterCamera != nullptr)
	{
		PlayerCameraManager->SetViewTarget(&CharacterCamera->GetViewTarget());
	}
}
