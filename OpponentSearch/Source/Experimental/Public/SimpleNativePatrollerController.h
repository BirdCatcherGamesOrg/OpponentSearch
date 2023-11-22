// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SimpleAIBlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SimpleNativePatrollerController.generated.h"

/**
 * Controller for a pawn that supports patrolling a shared search space
 * and reacting to any suspicious or alarming stimuli.
 */
UCLASS()
class EXPERIMENTALMODULE_API ASimpleNativePatrollerController : public AAIController
{
	GENERATED_BODY()

public:
	
	ASimpleNativePatrollerController();

	// Begin AAIController interface
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	// End AAIController interface
	
private:
	
	// AI handling components
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComponent;
	
	// Area-searching behavior
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UPatrolSharedSearchSpaceComponent> PatrolSharedSearchSpaceComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UNativeSharedSearchSpaceLogicComponent> NativeSharedSearchSpaceLogicComponent;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UAlarmStimulusBehaviorComponent> AlarmStimulusBehaviorComponent;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	float InvestigateSuspiciousLocationTime = 5.0;

	FDelegateHandle InvestigateSuspiciousLocationTimeHandle;

	FTSTicker::FDelegateHandle DelayedStartSearchHandle;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	float TimeToStayInAlarm = 15.0;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	float TimeToStayInSearch = 20.0;

	FBBKeyCachedAccessor<decltype(USimpleAIBlackboardData::AlarmStimulusLocationKey)::Type> AlarmStimulusLocationAccessor;
	
};
