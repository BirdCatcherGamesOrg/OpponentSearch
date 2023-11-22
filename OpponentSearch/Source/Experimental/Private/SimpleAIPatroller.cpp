// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "SimpleAIPatroller.h"

#include "BoxRadarVisibleComponent.h"
#include "CharacterSkeletalMeshUtils.h"
#include "CliqueHandler.h"
#include "SenseConfigPublisher.h"
#include "SimpleNativePatrollerController.h"
#include "SharedSearchSpaceAgent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "RadarVisibleComponent.h"

ASimpleAIPatroller::ASimpleAIPatroller()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CharacterSkeletalMeshUtils::SetDefaultSkeletalMeshProperties(*GetMesh());
	
	CliqueHandler = CreateDefaultSubobject<UCliqueHandler>(TEXT("CliqueHandler"));
	SenseConfigPublisher = CreateDefaultSubobject<USenseConfigPublisher>(TEXT("SenseConfigPublisher"));
	
	AIControllerClass = ASimpleNativePatrollerController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	SharedSearchSpaceAgent = CreateDefaultSubobject<USharedSearchSpaceAgent>(TEXT("SharedSearchSpaceAgent"));
	SharedSearchSpaceAgent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->MaxWalkSpeed = 200.0;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	BoxRadarVisibleComponent = CreateDefaultSubobject<UBoxRadarVisibleComponent>(TEXT("BoxRadarVisibleComponent"));
	BoxRadarVisibleComponent->SetupAttachment(GetRootComponent());
}

void ASimpleAIPatroller::PostInitializeComponents()
{
	//if (SenseAbilities->GetSenses().Contains())
	
	NativeHearing = NewObject<UAISenseConfig_Hearing>(this);
	const UAISenseConfig_Hearing* Default = GetDefault<UAISenseConfig_Hearing>();
	NativeHearing->Implementation = Default->Implementation;
    NativeHearing->HearingRange = 1000000;
    NativeHearing->DetectionByAffiliation.bDetectEnemies = true;
    NativeHearing->DetectionByAffiliation.bDetectFriendlies = true;
    NativeHearing->DetectionByAffiliation.bDetectNeutrals = true;

    NativeVision = NewObject<UAISenseConfig_Sight>(this);
	const UAISenseConfig_Sight* DefaultSight = GetDefault<UAISenseConfig_Sight>();
	NativeVision->Implementation = DefaultSight->Implementation;
    NativeVision->SightRadius = 500;
    NativeVision->LoseSightRadius = 1500;
    NativeVision->PeripheralVisionAngleDegrees = 45;
    NativeVision->DetectionByAffiliation.bDetectEnemies = true;
    NativeVision->DetectionByAffiliation.bDetectFriendlies = true;
    NativeVision->DetectionByAffiliation.bDetectNeutrals = true;
    
    SenseConfigPublisher->AddNativeSenses({NativeHearing, NativeVision});

	BoxRadarVisibleComponent->SetDependencies(*NativeVision);
	Super::PostInitializeComponents();
}
