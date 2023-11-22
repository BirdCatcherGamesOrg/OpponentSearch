// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "ReferencePerceptionCharacter.h"

#include "CVarBasedAISightTarget.h"
#include "ReferencePerceptionController.h"
#include "SenseConfigPublisher.h"
#include "TaggedSightTargetComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"


// Sets default values
AReferencePerceptionCharacter::AReferencePerceptionCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	SenseConfigPublisher = CreateDefaultSubobject<USenseConfigPublisher>(TEXT("SenseConfigPublisher"));
	AIControllerClass = AReferencePerceptionController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	CVarBasedAISightTarget = CreateDefaultSubobject<UCVarBasedAISightTarget>(TEXT("CVarBasedAISightTarget"));
	TaggedSightTarget = CreateDefaultSubobject<UTaggedSightTargetComponent>(TEXT("TaggedSightTarget"));
}

void AReferencePerceptionCharacter::PostInitializeComponents()
{
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
	
	TaggedSightTarget->AddAISightTargetInterfaces({CVarBasedAISightTarget});

	Super::PostInitializeComponents();
}

UAISense_Sight::EVisibilityResult AReferencePerceptionCharacter::CanBeSeenFrom(const FCanBeSeenFromContext& Context,
	FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested,
	float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	return TaggedSightTarget->CanBeSeenFrom(
		Context,
		OutSeenLocation,
		OutNumberOfLoSChecksPerformed,
		OutNumberOfAsyncLosCheckRequested,
		OutSightStrength,
		UserData,
		Delegate);
}


