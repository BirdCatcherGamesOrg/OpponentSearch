// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "SimpleAlarmingActor.h"
#include "AlarmStimulusCliqueTrackerSubsystem.h"
#include "CharacterSkeletalMeshUtils.h"
#include "CVarBasedAISightTarget.h"
#include "ExperimentalModuleLogCategory.h"
#include "TaggedSightTargetComponent.h"
#include "Xlog.h"
#include "Components/SphereComponent.h"

ASimpleAlarmingActor::ASimpleAlarmingActor()
{
	PrimaryActorTick.bCanEverTick = false;
	CharacterSkeletalMeshUtils::SetDefaultSkeletalMeshProperties(*GetMesh());
	TaggedSightTarget = CreateDefaultSubobject<UTaggedSightTargetComponent>(TEXT("TaggedSightTarget"));
	AcceptableRadiusBounds = CreateDefaultSubobject<USphereComponent>(TEXT("AcceptableRadiusBounds"));
	AcceptableRadiusBounds->SetSphereRadius(128);
	AcceptableRadiusBounds->SetupAttachment(GetMesh());
	AcceptableRadiusBounds->SetCanEverAffectNavigation(false);
	CVarBasedAISightTarget = CreateDefaultSubobject<UCVarBasedAISightTarget>(TEXT("CVarBasedAISightTarget"));
}

void ASimpleAlarmingActor::BeginPlay()
{
	Super::BeginPlay();
	UAlarmStimulusCliqueTrackerSubsystem* CliqueTrackerSubsystem =
		UWorld::GetSubsystem<UAlarmStimulusCliqueTrackerSubsystem>(GetWorld());
	/*
	CliqueTrackerSubsystem->OnCliqueEnteredAlarm().AddWeakLambda(this, [this]()
	{
		XLOG(LogExperimentalModule, Log, "A clique is in alarm");
	});
	*/
	TaggedSightTarget->AddAISightTargetInterfaces({CVarBasedAISightTarget});
}

UAISense_Sight::EVisibilityResult ASimpleAlarmingActor::CanBeSeenFrom(const FCanBeSeenFromContext& Context,
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
