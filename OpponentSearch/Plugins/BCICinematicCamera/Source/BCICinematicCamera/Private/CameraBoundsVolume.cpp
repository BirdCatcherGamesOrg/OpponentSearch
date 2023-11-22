// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "CameraBoundsVolume.h"

ACameraBoundsVolume::ACameraBoundsVolume()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot")));
}

void ACameraBoundsVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (HasAnyFlags(RF_ClassDefaultObject) || HasAnyFlags(RF_Transient))
	{
		return;
	}

	if (ValidArea == nullptr)
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Name = "ValidArea";
		ValidArea = GetWorld()->SpawnActor<ATriggerBox>(ActorSpawnParameters);
		ValidArea->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	if (MovableArea == nullptr)
	{
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.Name = "MovableArea";
		MovableArea = GetWorld()->SpawnActor<ATriggerBox>(ActorSpawnParameters);
		MovableArea->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

const FCameraBoundsAllowedTransforms& ACameraBoundsVolume::GetCameraBoundsAllowedTransforms() const
{
	return CameraBoundsAllowedTransforms;
}
