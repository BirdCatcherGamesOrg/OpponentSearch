// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PatrolSharedSearchSpaceComponent.generated.h"

/**
 * @brief Wrapper over a UNativeSharedSearchSpaceLogicComponent which implements patrolling behavior.
 * Subscribes to the logic components events and periodically waits before picking a new route.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHAREDSEARCHSPACE_API UPatrolSharedSearchSpaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	UPatrolSharedSearchSpaceComponent();

	/**
	 * @brief Kick-off patrol behavior for a controller using a logic component.
	 * @param AIController To use.
	 * @param InNativeSharedSearchSpaceLogicComponent To use. 
	 */
	void StartPatrolling(class AAIController& AIController, class UNativeSharedSearchSpaceLogicComponent& InNativeSharedSearchSpaceLogicComponent);

	/**
	 * @brief Stop patrolling.
	 * Unsubscribe from the logic components events and cancels any in-progress movements.
	 */
	void StopPatrolling();
	
private:
	
	UPROPERTY()
	TObjectPtr<class UNativeSharedSearchSpaceLogicComponent> NativeSharedSearchSpaceLogicComponent;

	UPROPERTY()
	TObjectPtr<class AAIController> Controller;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	float MinWaitTimeAtDestination = 1.0;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	float MaxWaitTimeAtDestination = 8.0;
	
	FDelegateHandle OnReachedNextWaypointHandle;
	
	FTSTicker::FDelegateHandle WaitAtDestinationTickerHandle;
	
};
