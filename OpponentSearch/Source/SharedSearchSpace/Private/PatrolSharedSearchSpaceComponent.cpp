// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "PatrolSharedSearchSpaceComponent.h"

#include "AIController.h"
#include "NativeSharedSearchSpaceLogicComponent.h"
#include "Navigation/PathFollowingComponent.h"

UPatrolSharedSearchSpaceComponent::UPatrolSharedSearchSpaceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPatrolSharedSearchSpaceComponent::StartPatrolling(AAIController& AIController, UNativeSharedSearchSpaceLogicComponent& InSharedSearchSpaceLogicComponent)
{
	NativeSharedSearchSpaceLogicComponent = &InSharedSearchSpaceLogicComponent;
	Controller = &AIController;
	OnReachedNextWaypointHandle = NativeSharedSearchSpaceLogicComponent->OnReachedNextWaypoint().AddWeakLambda(this, [this]()
	{
		const float WaitAtDestinationTime = 4;// FMath::RandRange(MinWaitTimeAtDestination, MaxWaitTimeAtDestination);
		const FTickerDelegate WaitAtDestination = FTickerDelegate::CreateWeakLambda(this, [this](float Delta)
		{
			NativeSharedSearchSpaceLogicComponent->MoveToNextWaypoint();
			return false;
		});
		WaitAtDestinationTickerHandle = FTSTicker::GetCoreTicker().AddTicker(WaitAtDestination, WaitAtDestinationTime);
		Controller->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(NativeSharedSearchSpaceLogicComponent);
	});
	NativeSharedSearchSpaceLogicComponent->MoveToNextWaypoint();
}

void UPatrolSharedSearchSpaceComponent::StopPatrolling()
{
	Controller->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(NativeSharedSearchSpaceLogicComponent);
	NativeSharedSearchSpaceLogicComponent->OnReachedNextWaypoint().Remove(OnReachedNextWaypointHandle);
	FTSTicker::GetCoreTicker().RemoveTicker(WaitAtDestinationTickerHandle);
	Controller->StopMovement();
}
