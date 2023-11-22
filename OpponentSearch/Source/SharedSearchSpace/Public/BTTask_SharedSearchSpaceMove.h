// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_SharedSearchSpaceMove.generated.h"

/**
 * @brief An extension of the MoveTo task which integrates with the USharedSearchSpaceSubsystem.
 * Useful for patrolling or searching for a hiding player.
 */
UCLASS()
class SHAREDSEARCHSPACE_API UBTTask_SharedSearchSpaceMove : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	UBTTask_SharedSearchSpaceMove();

	/**
	 * @copydoc UBTTask_MoveTo::InitializeFromAsset.
	 */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	/**
	 * @copydoc UBTTask_MoveTo::ExecuteTask
	 */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	static TAutoConsoleVariable<bool> CVarShowDebugNavigationPath;
	
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	FBlackboardKeySelector CliqueHandlerKey;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	FBlackboardKeySelector PatrollerSightBoxKey;
	
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	FBlackboardKeySelector MoveToLocation;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	FBlackboardKeySelector MoveToInitialAngleKey;
	
};
