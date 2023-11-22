// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "OpponentSearchPlayerController.h"
#include "Camera/CameraActor.h"
#include "CharacterCamera.h"
#include "MovementInputComponent.h"
#include "RadarUIModel.h"
#include "RadarWidget.h"
#include "TopLevelWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/NamedSlot.h"

AOpponentSearchPlayerController::AOpponentSearchPlayerController()
{
	MovementInputComponent = CreateDefaultSubobject<UMovementInputComponent>(TEXT("MovementInputComponent"));
}

void AOpponentSearchPlayerController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
	
	MovementInputComponent->SetPlayerController(*this);
	PushInputComponent(MovementInputComponent);
	
	const UCharacterCamera* CharacterCamera = PossessedPawn->FindComponentByClass<UCharacterCamera>();
	if (CharacterCamera != nullptr)
	{
		PlayerCameraManager->SetViewTarget(&CharacterCamera->GetViewTarget());
	}

	UTopLevelWidget* TopLevelWidget =
		Cast<UTopLevelWidget>(UUserWidget::CreateWidgetInstance(*this, TopLevelWidgetClass, "TopLevelWidget"));
    TopLevelWidget->AddToPlayerScreen();

	URadarUIModel* RadarUIModel = PossessedPawn->FindComponentByClass<URadarUIModel>();
	URadarWidget* RadarWidget = RadarUIModel->SetWidgetTree(TopLevelWidget->WidgetTree);

	TopLevelWidget->RadarSlot->SetContent(RadarWidget);
}