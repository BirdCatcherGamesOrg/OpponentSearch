// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "CharacterWithBoundableCamera.h"

#include "CharacterCamera.h"

ACharacterWithBoundableCamera::ACharacterWithBoundableCamera()
{
	PrimaryActorTick.bCanEverTick = false;
	CharacterCamera = CreateDefaultSubobject<UCharacterCamera>(TEXT("CharacterCamera"));
}

