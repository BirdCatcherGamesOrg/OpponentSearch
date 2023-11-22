// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SharedSearchSpaceSubsystem.h"
#include "NativeSharedSearchSpaceLogicComponent.generated.h"

/**
 * @brief A component for managed AIMovement integrating with the USharedSearchSpaceSubsystem.
 * This is a simple implementation that lacks BehaviorTree integration. This is suitable for simple behavior for a game
 * that is predominately implemented in C++ with little to no gameplay engineering Blueprints.
 * This component can be used in conjunction with other shared searchers so AI and non-AI searchers work simultaneously.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHAREDSEARCHSPACE_API UNativeSharedSearchSpaceLogicComponent : public USceneComponent
{
	GENERATED_BODY()

	/**
	 * @brief Callback for when the PathCorridor for movement changes.
	 */
	DECLARE_EVENT_OneParam(UNativeSharedSearchSpaceLogicComponent, FPathCorridorUpdatedSignature, const TArray<NavNodeRef>& PathCorridor);

	/**
	 * @brief Callback for when movement has completely finished.
	 * Multiple movement requests may happen before this is called. This allows searching along extended paths
	 * instead of stopping at every waypoint.
	 */
	DECLARE_EVENT(UNativeSharedSearchSpaceLogicComponent, FReachedNextWaypointSignature);
	
public:
	
	/**
	 * @brief Constructor.
	 */
	UNativeSharedSearchSpaceLogicComponent();

	/**
	 * @copydoc USceneComponent::OnRegister
	 */
	virtual void OnRegister() override;

	/**
	 * Sets dependencies.
	 * @param AIController To use. 
	 * @param InRecastNavMesh To use.
	 * @param InPatrollerSightBox To use.
	 * @param InAcceptanceRadius To use.
	 */
	void SetDependencies(class AAIController& AIController, const class ARecastNavMesh& InRecastNavMesh, class USharedSearchSpaceAgent& InPatrollerSightBox, const float InAcceptanceRadius);

	/**
	 * @brief Put in a new movement request.
	 */
	void MoveToNextWaypoint();

	/**
	 * @brief Callback for when the path corridor updates.
	 * @return Callback.
	 */
	FPathCorridorUpdatedSignature& OnPathCorridorUpdated();

	/**
	 * @brief Callback for when the next waypoint has been reached
	 * @return Callback.
	 */
	FReachedNextWaypointSignature& OnReachedNextWaypoint();
	
private:
	
	void ResumeMovement();
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	float AcceptanceRadius = 1;
	
	UPROPERTY()
	TObjectPtr<class AAIController> Controller;

	UPROPERTY()
	TObjectPtr<class USharedSearchSpaceAgent> PatrollerSightBox;
	
	UPROPERTY()
	TObjectPtr<const class ARecastNavMesh> RecastNavMesh;

	UPROPERTY()
	TMap<uint64, FNavNodePathResult> SegmentProbabilities;
	
	FNavPathSharedPtr CurrentNavPath;

	FPathCorridorUpdatedSignature PathCorridorUpdatedSignature;

	FReachedNextWaypointSignature ReachedNextWaypointSignature;
	
};
