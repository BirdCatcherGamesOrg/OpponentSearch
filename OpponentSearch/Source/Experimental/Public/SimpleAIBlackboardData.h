// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeBlackboardKey.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "SimpleAIBlackboardData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EXPERIMENTALMODULE_API USimpleAIBlackboardData : public UBlackboardData
{
	GENERATED_BODY()

public:

	virtual void PostInitProperties() override;
	
	const TNativeBlackboardKey<UBlackboardKeyType_Vector> AlarmStimulusLocationKey{TEXT("AlarmStimulusLocation"), false};
	
private:

	UPROPERTY()
	FBlackboardEntry AlarmStimulusLocationEntry;

	UPROPERTY()
	FBlackboardEntry CliqueHandlerEntry;

	UPROPERTY()
	FBlackboardEntry PatrollerSightBoxEntry;

	UPROPERTY()
	FBlackboardEntry MoveToLocationEntry;

	UPROPERTY()
	FBlackboardEntry MoveToInitialAngleEntry;
	
};
