// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "BoundedCameraActor.h"
#include "CameraBoundsVolume.h"

ABoundedCameraActor::ABoundedCameraActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot")));
}

void ABoundedCameraActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (HasAnyFlags(RF_ClassDefaultObject) || HasAnyFlags(RF_Transient))
	{
		return;
	}

	if (TopLevelCameraBoundsVolume == nullptr)
	{
		TopLevelCameraBoundsVolume = GetWorld()->SpawnActor<ACameraBoundsVolume>();
		TopLevelCameraBoundsVolume->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}
