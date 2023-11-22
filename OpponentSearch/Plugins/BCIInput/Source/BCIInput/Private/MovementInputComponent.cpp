// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "MovementInputComponent.h"
#include "BCIDynamicNamedInputSettings.h"
#include "GameFramework/InputSettings.h"

namespace
{
	void Move(AController& Controller, float Value, EAxis::Type AxisType)
	{
		if (Value != 0.0f)
		{
			// find out which way is forward
			const FRotator Rotation = Controller.GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(AxisType);
			Controller.GetPawn()->AddMovementInput(Direction, Value);
		}
	}
}

void UMovementInputComponent::SetPlayerController(AController& InController)
{
	Controller = &InController;
	const UBCIDynamicNamedInputSettings* DynamicNamedInputSettings = GetDefault<UBCIDynamicNamedInputSettings>();

	FInputAxisBinding MoveForwardAxisBinding(DynamicNamedInputSettings->MoveForward.AxisName);
	MoveForwardAxisBinding.AxisDelegate.GetDelegateForManualSet().BindWeakLambda(this, [this](float Value)
	{
		Move(*Controller, Value, EAxis::X);
	});
	AxisBindings.Emplace(MoveForwardAxisBinding);

	FInputAxisBinding MoveRightAxisBinding(DynamicNamedInputSettings->MoveRight.AxisName);
	MoveRightAxisBinding.AxisDelegate.GetDelegateForManualSet().BindWeakLambda(this, [this](float Value)
	{
		Move(*Controller, Value, EAxis::Y);
	});
	AxisBindings.Emplace(MoveRightAxisBinding);
}
