// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "SharedSearchSpaceSubsystem.h"

#include "ConsoleVariableUtilities.h"
#include "SharedSearchSpaceAgent.h"
#include "SharedSearchSpaceDeveloperSettings.h"
#include "SharedSearchSpaceLogCategory.h"
#include "SharedSearchSpaceLogicRecastQueryFilter.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavMesh/RecastNavMesh.h"
#include "SharedSearchSpaceStats.h"
#include "XLog.h"

DECLARE_CYCLE_STAT(TEXT("DissertationSim - SharedPatrolWaypointsSubsystem - CheckForDiscovery"), STAT_SharedPatrolWaypointsSubsystem_CheckForDiscovery, STATGROUP_DissertationSim);
DECLARE_CYCLE_STAT(TEXT("DissertationSim - SharedPatrolWaypointsSubsystem - PropagateProbability"), STAT_SharedPatrolWaypointsSubsystem_PropagateProbability, STATGROUP_DissertationSim);
DECLARE_CYCLE_STAT(TEXT("DissertationSim - SharedPatrolWaypointsSubsystem - TickPatrolWaypoints"), STAT_SharedPatrolWaypointsSubsystem_TickPatrolWaypoints, STATGROUP_DissertationSim);
DECLARE_CYCLE_STAT(TEXT("DissertationSim - SharedPatrolWaypointsSubsystem - GetNextWaypoint"), STAT_SharedPatrolWaypointsSubsystem_GetNextWaypoint, STATGROUP_DissertationSim);
DECLARE_CYCLE_STAT(TEXT("DissertationSim - SharedPatrolWaypointsSubsystem - SegmentHeapComparison"), STAT_SharedPatrolWaypointsSubsystem_SegmentHeapComparison, STATGROUP_DissertationSim);

TAutoConsoleVariable<bool> USharedSearchSpaceSubsystem::CVarDrawDebugVisualizations(
	AUTO_CVAR_NAME("DrawDebugVisualizations"),
	false,
	TEXT("Draw the debug sight box and line traces."));

namespace
{
	void CheckForDiscovery(const ARecastNavMesh* RecastNavMesh, const NavNodeRef PolyID, FSearchSpaceNavNodeInfo& SegmentInfo, USharedSearchSpaceAgent& Patroller, bool bDrawDebugVisualizations)
	{
		SCOPE_CYCLE_COUNTER(STAT_SharedPatrolWaypointsSubsystem_CheckForDiscovery);
		TRACE_CPUPROFILER_EVENT_SCOPE_STR("CheckForDiscovery");
		FVector NavPolyCenter;
		RecastNavMesh->GetPolyCenter(PolyID, NavPolyCenter);
		FRotator Rotation = Patroller.GetOwner()->GetActorRotation();
		FTransform Transform;
		Transform.SetLocation(Patroller.GetComponentLocation());
		Transform.SetRotation(FQuat::MakeFromEuler(Rotation.Euler() + FVector(0.0, 0.0, 45.0)));

		if (bDrawDebugVisualizations)
		{
			DrawDebugBox(Patroller.GetWorld(), Patroller.GetComponentLocation(), Patroller.GetBoxExtent(), FQuat::MakeFromEuler(Rotation.Euler() + FVector(0.0, 0.0, 45.0)), FColor::Red, false, 1, 0, 4);
		}
		if (UKismetMathLibrary::IsPointInBoxWithTransform(NavPolyCenter, Transform, Patroller.GetBoxExtent()))
		{

			FVector EyeLocation;
			Patroller.GetOwner()->GetActorEyesViewPoint(EyeLocation, Rotation);
			FCollisionQueryParams Params(NAME_None, false, Patroller.GetOwner());
			FHitResult HitResult;
			const bool bHasLineOfSight = Patroller.GetWorld()->LineTraceSingleByChannel(
				HitResult,
				EyeLocation,
				NavPolyCenter + FVector(0.0, 0.0, 5.0),
				ECollisionChannel::ECC_WorldStatic,
				Params);
			if (!bHasLineOfSight)
			{
				if (bDrawDebugVisualizations)
				{
					DrawDebugLine(Patroller.GetWorld(), NavPolyCenter  + FVector(0.0, 0.0, 5.0) , EyeLocation, FColor::Red, false, 1, 0, 1);
				}
				SegmentInfo.Probability = 0.0;
				SegmentInfo.OnPathCrossed.Broadcast(Patroller, PolyID);
			}
		}
	}

	void PropagateProbability(const ARecastNavMesh* RecastNavMesh, const NavNodeRef PolyID, const FSearchSpaceInfo& Waypoints, FSearchSpaceNavNodeInfo& SegmentInfo)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE_STR("PropagateProbability");
		// Case 1: Probability > 0
		if (SegmentInfo.Probability > 0.0)
		{
			SegmentInfo.Probability += 1;
			return;
		}

		// Case 2: Adjacent to a segment with a probability > 0.0
		TArray<NavNodeRef> Neighbors;
		RecastNavMesh->GetPolyNeighbors(PolyID, Neighbors);
		for (const NavNodeRef Neighbor : Neighbors)
		{
			if (Waypoints.NavNodes[Neighbor].Probability > 0.0)
			{
				SegmentInfo.Probability += 1;
				break;
			}
		}

	}
}

void USharedSearchSpaceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	LOG_INITIALIZE_SUBSYSTEM(LogSharedSearchSpace);
	Super::Initialize(Collection);
	const USharedSearchSpaceDeveloperSettings* PatrolBehaviorDeveloperSettings = GetDefault<USharedSearchSpaceDeveloperSettings>();
	SetSearchUpdateTickRate(PatrolBehaviorDeveloperSettings->PatrolTickRate);
}

void USharedSearchSpaceSubsystem::Deinitialize()
{
	LOG_DEINITIALIZE_SUBSYSTEM(LogSharedSearchSpace);
	Super::Deinitialize();
	FTSTicker::GetCoreTicker().RemoveTicker(TickSearchUpdateHandle);
}

void USharedSearchSpaceSubsystem::DrawDebugVisualizations(bool InbDrawDebugVisualizations)
{
	bDrawDebugVisualizations = InbDrawDebugVisualizations;
}

void USharedSearchSpaceSubsystem::SetSearchUpdateTickRate(float TickRate)
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickSearchUpdateHandle);
	if (TickRate == 0.0)
	{
		return;
	}
	
	const FTickerDelegate TickPatrolWaypoints = FTickerDelegate::CreateWeakLambda(this, [this](float Delta)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE_STR("TickPatrolWaypoints");
		for (auto& MeshWaypoints : SearchSpaces)
		{
			float MaxProbability = 0.0;
			for (auto& Segment : MeshWaypoints.Value.NavNodes)
			{
				PropagateProbability(MeshWaypoints.Key, Segment.Key, MeshWaypoints.Value, Segment.Value);
				for (USharedSearchSpaceAgent* Patroller : MeshWaypoints.Value.Patrollers)
				{
					if (Patroller == nullptr)
					{
						continue;
					}
					CheckForDiscovery(MeshWaypoints.Key, Segment.Key, Segment.Value, *Patroller, CVarDrawDebugVisualizations.GetValueOnGameThread());
				}
				MaxProbability = FMath::Max(Segment.Value.Probability, MaxProbability);
			}

			if (MaxProbability < 0.2)
			{
				// TODO: Normalize.
			}
		}
		SegmentProbabilitiesUpdatedSignature.Broadcast();
		return true;
	});
	
	TickSearchUpdateHandle = FTSTicker::GetCoreTicker().AddTicker(TickPatrolWaypoints, TickRate);

	const FTickerDelegate TickGuardMinUpdate = FTickerDelegate::CreateWeakLambda(this, [this](float Delta)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE_STR("CacheGuardMins")
		for (auto& MeshWaypoints : SearchSpaces)
		{
			SharedConstNavQueryFilter = MakeShared<FNavigationQueryFilter>();
			const TSharedRef<FSharedSearchSpaceLogicRecastQueryFilter> PatrolLogicRecastQueryFilter =
				MakeShared<FSharedSearchSpaceLogicRecastQueryFilter>(MeshWaypoints.Value, 1);
			SharedConstNavQueryFilter->SetFilterImplementation(TSharedPtr<FSharedSearchSpaceLogicRecastQueryFilter>(PatrolLogicRecastQueryFilter).Get());
			
			// Take on 1/60th of all the waypoints per tick to amortize costs.
			for (int i = 0; GuardMinUpdateWaypointIndex + i < MeshWaypoints.Value.CachedKeys.Num() && i < 1 + (MeshWaypoints.Value.CachedKeys.Num() / 600); i++, GuardMinUpdateWaypointIndex++)
			{
				FSearchSpaceNavNodeInfo& Segment = MeshWaypoints.Value.NavNodes[MeshWaypoints.Value.CachedKeys[GuardMinUpdateWaypointIndex + i]];
				for (USharedSearchSpaceAgent* PatrollerSightBox :  MeshWaypoints.Value.Patrollers)
				{
					const double SelfEuclideanDist = FVector::DistXY(PatrollerSightBox->GetOwner()->GetActorLocation(), Segment.NavNodeCenter);
					if (SelfEuclideanDist > 2000)
					{
						Segment.CachedPaths.Emplace(PatrollerSightBox, FPathFindingResult());
					}

					const FPathFindingQuery Query(
						PatrollerSightBox,
						*MeshWaypoints.Key,
						PatrollerSightBox->GetOwner()->GetActorLocation(),
						Segment.NavNodeCenter,
						SharedConstNavQueryFilter,
						nullptr);
					FPathFindingResult Path = MeshWaypoints.Key->FindPath({}, Query);
					if (Segment.CachedGuardMin == -1)
					{
						Segment.CachedGuardMin = Path.Path->GetLength();
					}
					else
					{
						Segment.CachedGuardMin = FMath::Min(Segment.CachedGuardMin, Path.Path->GetLength());
					}
					Segment.CachedPaths.Add(PatrollerSightBox, Path);
				}
			}
			
			// Reset once we've reached the end of all cached keys.
			if (GuardMinUpdateWaypointIndex == MeshWaypoints.Value.CachedKeys.Num())
			{
				GuardMinUpdateWaypointIndex = 0;
			}
		}
		return true;
	});
	
	//TickGuardMinUpdateHandle = FTSTicker::GetCoreTicker().AddTicker(TickGuardMinUpdate, 0);
}

bool USharedSearchSpaceSubsystem::SetNodesOnPaths(USharedSearchSpaceAgent& PatrollerSightBox, const ARecastNavMesh& RecastNavMesh, const TArray<NavNodeRef>& NavPathPoints, FPathCrossedSignature::FDelegate OnPathCrossed)
{
	FSearchSpaceInfo* Waypoints = SearchSpaces.Find(&RecastNavMesh);
	if (Waypoints == nullptr)
	{
		return false;
	}
		
	for (const NavNodeRef& NavPathPoint : NavPathPoints)
	{
		Waypoints->NavNodes[NavPathPoint].SegmentPatrollers.Add(PatrollerSightBox);
		Waypoints->NavNodes[NavPathPoint].OnPathCrossed.Add(OnPathCrossed);
	}
	
	return true;
}

bool USharedSearchSpaceSubsystem::UnsetNodesOnPaths(const USharedSearchSpaceAgent& PatrollerSightBox, const ARecastNavMesh& RecastNavMesh, const TArray<NavNodeRef>& NavPathPoints)
{
	FSearchSpaceInfo* Waypoints = SearchSpaces.Find(&RecastNavMesh);
	if (Waypoints == nullptr)
	{
		return false;
	}
	
	for (const NavNodeRef& NavPathPoint : NavPathPoints)
	{
		Waypoints->NavNodes[NavPathPoint].SegmentPatrollers.Remove(&PatrollerSightBox);
		Waypoints->NavNodes[NavPathPoint].OnPathCrossed.RemoveAll(&PatrollerSightBox);
	}

	return true;
}

void USharedSearchSpaceSubsystem::CreateSearchSpaces(USharedSearchSpaceAgent& PatrollerSightBox, const ARecastNavMesh& RecastNavMesh)
{
	// Add the patroller, emplace the RecastNavMesh only if it's not been registered with the subsystem yet.
	FSearchSpaceInfo* Waypoints = SearchSpaces.Find(RecastNavMesh);
	if (Waypoints != nullptr)
	{
		Waypoints->Patrollers.Add(PatrollerSightBox);
		return;
	}

	// New RecastNavMesh. Initialize probabilities for all segments and add the patroller as a singleton.
	FSearchSpaceInfo NewWaypoints;
	NewWaypoints.Patrollers.Add(PatrollerSightBox);

	// Curious: What are the tiles and polys?
	for (int i = 0; i < RecastNavMesh.GetNavMeshTilesCount(); i++)
	{
		TArray<FNavPoly> NavPolys;
		RecastNavMesh.GetPolysInTile(i, NavPolys);
		for (const FNavPoly& NavPoly : NavPolys)
		{
			// Add between 0.0 and 1.0 to create a bit of randomness to the start of the search space
			// Otherwise, every guard will begin every map on the same patrol routes.
			NewWaypoints.NavNodes.Emplace(NavPoly.Ref, {FMath::RandRange(0.0, 1000.0), NavPoly.Center});
			NewWaypoints.CachedKeys.Add(NavPoly.Ref);
		}
	}
	
	SearchSpaces.Emplace(RecastNavMesh, NewWaypoints);
}

bool USharedSearchSpaceSubsystem::GetWaypointScores(const USharedSearchSpaceAgent& PatrollerSightBox, const ARecastNavMesh& RecastNavMesh, const FVector& CurrentLocation, TMap<NavNodeRef, FNavNodePathResult>& SegmentProbabilities)
{
	TRACE_CPUPROFILER_EVENT_SCOPE_STR("GetNextWaypoint")
	const FSearchSpaceInfo* Waypoints = SearchSpaces.Find(&RecastNavMesh);
	if (Waypoints == nullptr)
	{
		return false;
	}
	
	SharedConstNavQueryFilter = MakeShared<FNavigationQueryFilter>();
	const TSharedRef<FSharedSearchSpaceLogicRecastQueryFilter> PatrolLogicRecastQueryFilter =
		MakeShared<FSharedSearchSpaceLogicRecastQueryFilter>(*Waypoints, 1);
	SharedConstNavQueryFilter->SetFilterImplementation(TSharedPtr<FSharedSearchSpaceLogicRecastQueryFilter>(PatrolLogicRecastQueryFilter).Get());
	
	Algo::Transform(Waypoints->NavNodes.Array(), SegmentProbabilities, [this, InPatrollerSightBox = &PatrollerSightBox, RecastNavMesh = &RecastNavMesh, Waypoints](auto& Segment) mutable
	{
		TRACE_CPUPROFILER_EVENT_SCOPE_STR("GetNextWaypointSinglePoint")
/*
		const FPathFindingResult* QuerierFindPathResult = Segment.Value.CachedPaths.Find(InPatrollerSightBox);
		FPathFindingResult Result;
		double MinGuardPathLength = Segment.Value.CachedGuardMin;
		if (QuerierFindPathResult == nullptr)
		{
			FNavNodePathResult NavNodePathResult;
			NavNodePathResult.Fitness = 0;
			return TTuple<NavNodeRef, FNavNodePathResult>(Segment.Key, NavNodePathResult);
		}
		if (QuerierFindPathResult != nullptr)
		{
			Result = *QuerierFindPathResult;
		}
*/
		
		const double SelfEuclideanDist = FVector::DistXY(InPatrollerSightBox->GetOwner()->GetActorLocation(), Segment.Value.NavNodeCenter);
		if (SelfEuclideanDist > 2000)
		{
			FNavNodePathResult NavNodePathResult;
			NavNodePathResult.Fitness = 0;
			return TTuple<NavNodeRef, FNavNodePathResult>(Segment.Key, NavNodePathResult);
		}

		const FPathFindingQuery Query(
			InPatrollerSightBox,
			*RecastNavMesh,
			InPatrollerSightBox->GetOwner()->GetActorLocation(),
			Segment.Value.NavNodeCenter,
			SharedConstNavQueryFilter);
		FPathFindingResult Result = RecastNavMesh->FindPath({}, Query);
		
		double MinGuardPathLength = -1;
		for (const USharedSearchSpaceAgent* PatrollerSightBox : Waypoints->Patrollers)
		{
			if (PatrollerSightBox == InPatrollerSightBox)
			{
				continue;
			}

			const double EuclideanDist = FVector::DistXY(PatrollerSightBox->GetOwner()->GetActorLocation(), Segment.Value.NavNodeCenter);
			if (MinGuardPathLength != - 1 && EuclideanDist > MinGuardPathLength)
			{
				continue;
			}
			
			double PathLength;
			RecastNavMesh->CalcPathLength(
				PatrollerSightBox->GetOwner()->GetActorLocation(),
				Segment.Value.NavNodeCenter,
				PathLength,
				SharedConstNavQueryFilter,
				InPatrollerSightBox);
			if (MinGuardPathLength == -1)
			{
				MinGuardPathLength = PathLength;
			}
			else
			{
				MinGuardPathLength = FMath::Min(MinGuardPathLength, PathLength);
			}
		}

		const float Fitness = Result.Path->GetCost() + (-0.8 * Result.Path->GetLength()) + MinGuardPathLength; 
		const FNavNodePathResult NavNodePathResult{Result, Fitness};
		return TTuple<NavNodeRef, FNavNodePathResult>(Segment.Key, NavNodePathResult);
	});
	
	return true;
}


const FSearchSpaceInfo* USharedSearchSpaceSubsystem::GetSearchSpaceInfo(const ARecastNavMesh& RecastNavMesh) const
{
	return SearchSpaces.Find(RecastNavMesh);
}

bool USharedSearchSpaceSubsystem::SetFocusedSegment(const ARecastNavMesh& RecastNavMesh, const FVector& FocusLocation)
{
	FSearchSpaceInfo* Waypoints = SearchSpaces.Find(&RecastNavMesh);
	if (Waypoints == nullptr)
	{
		return false;
	}

	const NavNodeRef NearestPoly = RecastNavMesh.FindNearestPoly(FocusLocation, {100.0, 100.0, 100.0});
	if (Waypoints == nullptr)
	{
		return false;
	}

	for (auto& SegmentInfo : Waypoints->NavNodes)
	{
		SegmentInfo.Value.Probability = 1.0;
	}
	
	FSearchSpaceNavNodeInfo* SegmentInfo = Waypoints->NavNodes.Find(NearestPoly);
	FVector NavPolyCenter;
	RecastNavMesh.GetPolyCenter(NearestPoly, NavPolyCenter);
	if (bDrawDebugVisualizations)
	{
		DrawDebugSphere(GetWorld(), NavPolyCenter, 5.0, 10, FColor::Red, false, 5.0);
	}
	PropagateProbability(&RecastNavMesh, NearestPoly, *Waypoints, *SegmentInfo);

	for (const USharedSearchSpaceAgent* PointPatroller : Waypoints->Patrollers)
	{
		if (PointPatroller == nullptr)
		{
			continue;
		}
		//PointPatroller->MoveToNextWaypoint();
	}
	return true;
}

bool USharedSearchSpaceSubsystem::IsTrackedNavMesh(const ARecastNavMesh& RecastNavMesh) const
{
	return SearchSpaces.Contains(&RecastNavMesh);
}

void USharedSearchSpaceSubsystem::GetTrackedNavMeshes(TArray<const ARecastNavMesh*>& TrackedNavMeshes) const
{
	Algo::Transform(SearchSpaces, TrackedNavMeshes, [](const auto& SearchSpace)
	{
		return SearchSpace.Key;
	});
}

USharedSearchSpaceSubsystem::FTrackedNavMeshesUpdatedSignature& USharedSearchSpaceSubsystem::OnTrackedNavMeshesUpdated()
{
	return TrackedNavMeshesUpdatedSignature;
}

USharedSearchSpaceSubsystem::FSegmentProbabilitiesUpdatedSignature& USharedSearchSpaceSubsystem::OnSegmentProbabilitiesUpdated()
{
	return SegmentProbabilitiesUpdatedSignature;
}
