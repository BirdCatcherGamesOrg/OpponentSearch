// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SimpleAIBlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SimpleAIPatrollerController.generated.h"

UCLASS(Blueprintable)
class EXPERIMENTALMODULE_API ASimpleAIPatrollerController : public AAIController
{
	GENERATED_BODY()

public:

	ASimpleAIPatrollerController();

	virtual void PostInitializeComponents() override;

	virtual void OnPossess(APawn* InPawn) override;

private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComponent;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TObjectPtr<class UBlackboardData> BlackboardData;
	
	FBBKeyCachedAccessor<decltype(USimpleAIBlackboardData::AlarmStimulusLocationKey)::Type> AlarmStimulusLocationAccessor;
	
};
