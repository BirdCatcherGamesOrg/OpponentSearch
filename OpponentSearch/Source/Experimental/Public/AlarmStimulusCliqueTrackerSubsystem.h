// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CliqueHandle.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AlarmStimulusCliqueTrackerSubsystem.generated.h"

DECLARE_EVENT_OneParam(UAlarmStimulusCliqueTrackerSubsystem, FCliqueEnteredAlarmSignature, class AActor& AlarmStimulusTarget);
DECLARE_EVENT(UAlarmStimulusCliqueTrackerSubsystem, FCliqueExitedAlarmSignature);

USTRUCT()
struct FAlarmStimulusCliqueData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<class UAlarmStimulusBehaviorComponent>> CliqueMembers;

	UPROPERTY()
	int32 NumberSensingTarget = 0;
	
	UPROPERTY()
	TObjectPtr<class AActor> AlarmStimulusTarget = nullptr;

	FCliqueEnteredAlarmSignature OnCliqueEnteredAlarm;
	
};

/**
 * 
 */
UCLASS()
class EXPERIMENTALMODULE_API UAlarmStimulusCliqueTrackerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()


	
public:

	void RegisterAlarmStimulusBehavior(const FCliqueHandle& Clique, class UAlarmStimulusBehaviorComponent& AlarmStimulusBehavior);
	
	void RegisterAlarmingLocation(const FCliqueHandle& Clique, class UAlarmStimulusBehaviorComponent& AlarmStimulusBehavior, class AActor& AlarmStimulusTarget);

	void RemoveAlarmingLocation(const class UCliqueHandler& Clique, class UAlarmStimulusBehaviorComponent& AlarmStimulusBehavior, const FVector& LastKnownAlarmStimulusTargetLocation);
	
	FCliqueEnteredAlarmSignature& OnCliqueEnteredAlarm() { return OnCliqueEnteredAlarmSignature; }

	FCliqueExitedAlarmSignature& OnCliqueExitedAlarm() { return OnCliqueExitedAlarmSignature; }
	
private:

	UPROPERTY()
	TMap<FCliqueHandle, FAlarmStimulusCliqueData> AlarmStimulusCliques;

	FCliqueEnteredAlarmSignature OnCliqueEnteredAlarmSignature;

	FCliqueExitedAlarmSignature OnCliqueExitedAlarmSignature;
	
};
