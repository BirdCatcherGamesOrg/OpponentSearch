// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "ReferenceSharedSearchSpaceBlackboard.generated.h"

/**
 * 
 */
UCLASS()
class SHAREDSEARCHSPACE_API UReferenceSharedSearchSpaceBlackboard : public UBlackboardData
{
	GENERATED_BODY()

public:

	virtual void PostInitProperties() override;
	
private:

	UPROPERTY()
	FBlackboardEntry CliqueHandlerEntry;

	UPROPERTY()
	FBlackboardEntry PatrollerSightBoxEntry;

	UPROPERTY()
	FBlackboardEntry MoveToLocationEntry;
	
};
