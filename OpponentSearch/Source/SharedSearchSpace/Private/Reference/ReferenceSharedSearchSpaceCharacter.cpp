// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "ReferenceSharedSearchSpaceCharacter.h"
#include "CharacterSkeletalMeshUtils.h"
#include "ReferenceSharedSearchSpaceController.h"
#include "CliqueHandler.h"
#include "SharedSearchSpaceAgent.h"

// Sets default values
AReferenceSharedSearchSpaceCharacter::AReferenceSharedSearchSpaceCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	CharacterSkeletalMeshUtils::SetDefaultSkeletalMeshProperties(*GetMesh());
	CliqueHandler = CreateDefaultSubobject<UCliqueHandler>(TEXT("CliqueHandler"));
	AIControllerClass = AReferenceSharedSearchSpaceController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	PatrollerSightBox = CreateDefaultSubobject<USharedSearchSpaceAgent>(TEXT("PatrollerSightBox"));
	PatrollerSightBox->SetupAttachment(RootComponent);
}

