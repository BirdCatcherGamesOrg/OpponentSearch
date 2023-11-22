// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "NativeSharedSearchSpaceLogicComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "SharedSearchSpaceAgent.h"
#include "SearchSpaceLogicNavigationQueryFilter.h"
#include "SharedSearchSpaceSubsystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavMesh/RecastNavMesh.h"
#include "SharedSearchSpaceStats.h"

DECLARE_CYCLE_STAT(TEXT("DissertationSim - PatrolLogicComponent - OnRequestFinished"), STAT_PatrolLogicComponent_OnRequestFinished, STATGROUP_DissertationSim);

namespace
{
	constexpr FPathFollowingResultFlags::Type CHANGE_WAYPOINT_FLAG = FPathFollowingResultFlags::NewRequest;

	bool MoveTo(
		AAIController& Controller,
		USharedSearchSpaceSubsystem& SharedSearchSpaceSubsystem,
		const USharedSearchSpaceAgent& PatrollerSightBox,
		TMap<NavNodeRef, FNavNodePathResult> SegmentProbabilities,
		const ARecastNavMesh& RecastNavMesh,
		float AcceptanceRadius,
		FNavPathSharedPtr& CurrentNavPath)
	{
		const FVector CurrentLocation = Controller.GetNavAgentLocation();
		SharedSearchSpaceSubsystem.GetWaypointScores(PatrollerSightBox, RecastNavMesh, CurrentLocation, SegmentProbabilities);

		if (SegmentProbabilities.Num() == 0)
		{
			return false;
		}
		
		FVector NextWaypoint;
		float MaxScore = -1;
		for (const auto Waypoint : SegmentProbabilities)
		{
			if (Waypoint.Value.Fitness > MaxScore)
			{
				MaxScore = Waypoint.Value.Fitness;
				RecastNavMesh.GetPolyCenter(Waypoint.Key, NextWaypoint);
			}
		}
		
		// Optimization opportunity - This is technically not required, if the nav mesh never changes...
		SegmentProbabilities.Reset();
		// Attempt to move
		FAIMoveRequest SimplePatrol(NextWaypoint);
		SimplePatrol.SetNavigationFilter(USearchSpaceLogicNavigationQueryFilter::StaticClass());
		SimplePatrol.SetAcceptanceRadius(AcceptanceRadius);
		const FPathFollowingRequestResult NewRequestResult = Controller.MoveTo(SimplePatrol, &CurrentNavPath);

		// Case can be that the goal is unreachable, or already within the acceptance radius
		if (NewRequestResult.Code != EPathFollowingRequestResult::RequestSuccessful)
		{
			return false;
		}
		return true;
	}
}

UNativeSharedSearchSpaceLogicComponent::UNativeSharedSearchSpaceLogicComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNativeSharedSearchSpaceLogicComponent::OnRegister()
{
	Super::OnRegister();
}

void UNativeSharedSearchSpaceLogicComponent::SetDependencies(AAIController& AIController, const ARecastNavMesh& InRecastNavMesh, USharedSearchSpaceAgent& InPatrollerSightBox, const float InAcceptanceRadius)
{
	Controller = &AIController;
	RecastNavMesh = &InRecastNavMesh;
	PatrollerSightBox = &InPatrollerSightBox;
	AcceptanceRadius = InAcceptanceRadius;
	
	USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem = AIController.GetWorld()->GetSubsystem<USharedSearchSpaceSubsystem>();
	SharedSearchSpaceSubsystem->CreateSearchSpaces(*PatrollerSightBox, *RecastNavMesh);
}

void UNativeSharedSearchSpaceLogicComponent::ResumeMovement()
{
	Controller->ResumeMove(Controller->GetCurrentMoveRequestID());
	const FNavMeshPath* NavMeshPath = CurrentNavPath->CastPath<FNavMeshPath>();
	if (NavMeshPath != nullptr)
	{
		USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem = GetWorld()->GetSubsystem<USharedSearchSpaceSubsystem>();
		const FPathCrossedSignature::FDelegate OnPathCrossed = FPathCrossedSignature::FDelegate::CreateWeakLambda(this, [this, SharedSearchSpaceSubsystem](USharedSearchSpaceAgent& SeenBy, uint64 NavNode)
		{
			if (PatrollerSightBox != &SeenBy)
			{
				Controller->GetPathFollowingComponent()->AbortMove(*this, CHANGE_WAYPOINT_FLAG);
			}
		});
		SharedSearchSpaceSubsystem->SetNodesOnPaths(*PatrollerSightBox, *RecastNavMesh, NavMeshPath->PathCorridor, OnPathCrossed);
		PathCorridorUpdatedSignature.Broadcast(CurrentNavPath->CastPath<FNavMeshPath>()->PathCorridor);
	}
}

void UNativeSharedSearchSpaceLogicComponent::MoveToNextWaypoint()
{
	if (CurrentNavPath != nullptr)
	{
		USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem = GetWorld()->GetSubsystem<USharedSearchSpaceSubsystem>();
		const FNavMeshPath* NavMeshPath = CurrentNavPath->CastPath<FNavMeshPath>();
		SharedSearchSpaceSubsystem->UnsetNodesOnPaths(*PatrollerSightBox, *RecastNavMesh, NavMeshPath->PathCorridor);
	}
	
	if (!Controller->GetPathFollowingComponent()->OnRequestFinished.IsBoundToObject(this))
	{
		Controller->GetPathFollowingComponent()->OnRequestFinished.AddWeakLambda(this, [this](FAIRequestID RequestID, const FPathFollowingResult& Result)
		{
			USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem = Controller->GetWorld()->GetSubsystem<USharedSearchSpaceSubsystem>();
			// If a path was chosen, then remove it from the shared search space.
			if (CurrentNavPath != nullptr)
			{
				const FNavMeshPath* NavMeshPath = CurrentNavPath->CastPath<FNavMeshPath>();
				SharedSearchSpaceSubsystem->UnsetNodesOnPaths(*PatrollerSightBox, *RecastNavMesh, NavMeshPath->PathCorridor);
			}

			if (Result.HasFlag(FPathFollowingResultFlags::MovementStop) || Result.HasFlag(CHANGE_WAYPOINT_FLAG) || Result.HasFlag(FPathFollowingResultFlags::AlreadyAtGoal))
			{
				ReachedNextWaypointSignature.Broadcast();
				return;
			}

			// For smooth movement, if the next path is to continue walking forward - do so.
			// Look ahead to see if the next path is a smooth continuation of the current path.
			const bool bMoveToSucceeded = MoveTo(*Controller, *SharedSearchSpaceSubsystem, *PatrollerSightBox, SegmentProbabilities, *RecastNavMesh, AcceptanceRadius, CurrentNavPath);
			if (!bMoveToSucceeded || CurrentNavPath == nullptr)
			{
				ReachedNextWaypointSignature.Broadcast();
				return;
			}
				
			// Immediately pause to determine if we should continue down this path or not
			Controller->PauseMove(Controller->GetCurrentMoveRequestID());
			Controller->ClearFocus(EAIFocusPriority::Move);

			// Case is the next node is a direct continuation of the current path.
			if (PatrollerSightBox->GetOwner()->GetActorForwardVector().Dot(CurrentNavPath->GetSegmentDirection(0)) > 0.2)
			{
				ResumeMovement();
				return;
			}

			// Case is that the next node is not a direct continuation of the current path.
			// Therefore, the waypoint is "reached".
			ReachedNextWaypointSignature.Broadcast();
		});
	}
	
	if (Controller->GetMoveStatus() == EPathFollowingStatus::Type::Moving)
	{
		Controller->GetPathFollowingComponent()->AbortMove(*this, CHANGE_WAYPOINT_FLAG);
		return;
	}

	// Starting on the next frame prevents stack overflow for the AlreadyAtGoal case.
	const FTickerDelegate OnNextFrame = FTickerDelegate::CreateWeakLambda(this, [this](float Delta)
	{
		USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem = GetWorld()->GetSubsystem<USharedSearchSpaceSubsystem>();

		const bool bMoveToSucceeded = MoveTo(*Controller, *SharedSearchSpaceSubsystem, *PatrollerSightBox, SegmentProbabilities, *RecastNavMesh, AcceptanceRadius, CurrentNavPath);
		if (!bMoveToSucceeded)
		{
			//ReachedNextWaypointSignature.Broadcast();
			return false;
		}
	
		// Otherwise, set that the path is currently being searched
		const FNavMeshPath* NavMeshPath = CurrentNavPath->CastPath<FNavMeshPath>();
		if (NavMeshPath != nullptr)
		{
			const FPathCrossedSignature::FDelegate OnPathCrossed = FPathCrossedSignature::FDelegate::CreateWeakLambda(this, [this, SharedSearchSpaceSubsystem](USharedSearchSpaceAgent& SeenBy, uint64 NavNode)
			{
				if (PatrollerSightBox != &SeenBy)
				{
					Controller->GetPathFollowingComponent()->AbortMove(*this, CHANGE_WAYPOINT_FLAG);
				}
			});
			SharedSearchSpaceSubsystem->SetNodesOnPaths(*PatrollerSightBox, *RecastNavMesh, NavMeshPath->PathCorridor, OnPathCrossed);
			PathCorridorUpdatedSignature.Broadcast(CurrentNavPath->CastPath<FNavMeshPath>()->PathCorridor);
		}
		
		return false;
	});
	
	FTSTicker::GetCoreTicker().AddTicker(OnNextFrame, 0.0f);
}

UNativeSharedSearchSpaceLogicComponent::FPathCorridorUpdatedSignature& UNativeSharedSearchSpaceLogicComponent::OnPathCorridorUpdated()
{
	return PathCorridorUpdatedSignature;
}

UNativeSharedSearchSpaceLogicComponent::FReachedNextWaypointSignature& UNativeSharedSearchSpaceLogicComponent::OnReachedNextWaypoint()
{
	return ReachedNextWaypointSignature;
}
