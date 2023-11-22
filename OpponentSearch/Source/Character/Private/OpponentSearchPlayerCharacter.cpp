// Copyright Epic Games, Inc. All Rights Reserved.

#include "OpponentSearchPlayerCharacter.h"

#include "CharacterCamera.h"
#include "CharacterRadar.h"
#include "CharacterSkeletalMeshUtils.h"
#include "CVarBasedAISightTarget.h"
#include "TaggedSightTargetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AOpponentSearchPlayerCharacter::AOpponentSearchPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CharacterSkeletalMeshUtils::SetDefaultSkeletalMeshProperties(*GetMesh());

	CharacterRadarComponent = CreateDefaultSubobject<UCharacterRadarComponent>(TEXT("CharacterRadarComponent"));
	CharacterRadarComponent->SetupAttachment(GetRootComponent());
	
	TaggedSightTarget = CreateDefaultSubobject<UTaggedSightTargetComponent>(TEXT("TaggedSightTarget"));
	CVarBasedAISightTarget = CreateDefaultSubobject<UCVarBasedAISightTarget>(TEXT("CVarBasedAISightTarget"));
	
	CharacterCamera = CreateDefaultSubobject<UCharacterCamera>(TEXT("CharacterCamera"));
	CharacterCamera->SetupAttachment(GetRootComponent());
}

UAISense_Sight::EVisibilityResult AOpponentSearchPlayerCharacter::CanBeSeenFrom(const FCanBeSeenFromContext& Context,
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
