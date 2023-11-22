// Copyright 2021 Bird Catcher Interactive. All Rights Reserved.

#include "BCIInputReferenceController.h"

#include "MovementInputComponent.h"

ABCIInputReferenceController::ABCIInputReferenceController()
{
	MovementInputComponent = CreateDefaultSubobject<UMovementInputComponent>(TEXT("MovementInputComponent"));
}

void ABCIInputReferenceController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
	
	MovementInputComponent->SetPlayerController(*this);
}
