// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"
#include "SharedSearchSpaceSubsystem.generated.h"

/**
 * @brief Called when a NavNode is seen by a patroller.
 */
DECLARE_EVENT_TwoParams(USharedSearchSpaceSubsystem, FPathCrossedSignature, class USharedSearchSpaceAgent& SeenBy, uint64 NavNode)

/**
 * @brief Probability information for a single nav node.
 */
USTRUCT()
struct SHAREDSEARCHSPACE_API FSearchSpaceNavNodeInfo
{
	GENERATED_BODY()

	FSearchSpaceNavNodeInfo() = default;

	/**
	 * @brief Constructor.
	 * @param NewProbability Initial probability.
	 * @param NewNavNodeCenter NavNodeCenter
	 */
	FSearchSpaceNavNodeInfo(double NewProbability, const FVector& NewNavNodeCenter) : Probability(NewProbability), NavNodeCenter(NewNavNodeCenter){}

	/**
	 * @brief Current probability.
	 */
	UPROPERTY()
	float Probability = 0.0;

	UPROPERTY()
	FVector NavNodeCenter = FVector::Zero();

	UPROPERTY()
	double CachedGuardMin = -1;

	TMap<TObjectPtr<class USharedSearchSpaceAgent>, FPathFindingResult> CachedPaths;
	
	/**
	 * @brief All patrollers currently navigating a path through the nav node.
	 */
	UPROPERTY()
	TSet<TObjectPtr<class USharedSearchSpaceAgent>> SegmentPatrollers;

	/**
	 * @brief Event when the nav node is seen by any patroller.
	 */
	FPathCrossedSignature OnPathCrossed;
	
};

/**
 * @brief All NavNode info for a ARecastNavMesh.
 */
USTRUCT()
struct SHAREDSEARCHSPACE_API FSearchSpaceInfo
{
	GENERATED_BODY()

	/**
	 * @brief All patrollers for an ARecastNavMesh.
	 */
	UPROPERTY()
	TArray<TObjectPtr<class USharedSearchSpaceAgent>> Patrollers;
		
	/**
	 * @brief The segment info for all NavNodes.
	 */
	TMap<NavNodeRef, FSearchSpaceNavNodeInfo> NavNodes;

	TArray<NavNodeRef> CachedKeys;
};

USTRUCT()
struct FNavNodePathResult
{
	GENERATED_BODY()

	FPathFindingResult PathFindingResult;
	
	UPROPERTY()
	float Fitness = 0.0;
	
};

/**
 * @brief Main logic for coordinating search space segment probabilities across an entire ARecastNavMesh.
 * Searchers will register for a
 */
UCLASS()
class SHAREDSEARCHSPACE_API USharedSearchSpaceSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(USharedSearchSpaceSubsystem, FTrackedNavMeshesUpdatedSignature, const ARecastNavMesh& RecastNavMesh)

	DECLARE_EVENT(USharedSearchSpaceSubsystem, FSegmentProbabilitiesUpdatedSignature)
	
public:
	
	/**
	 * @copydoc UWorldSubsystem::Initialize
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @copydoc UWorldSubsystem::Deinitialize
	 */
	virtual void Deinitialize() override;

	virtual void DrawDebugVisualizations(bool InbDrawDebugVisualizations);

	/**
	 * @brief Setter for the rate of checking waypoint vision.
	 * @param TickRate Rate in seconds
	 */
	void SetSearchUpdateTickRate(float TickRate);

	/**
	 * @brief Mark the nodes for a shared search space as actively being searched by an agent.
	 * The number of agents with a path through a nav node determines the pathfinding score for the node.
	 * This prevents agents from crossing paths with each other.
	 * Additionally, any agents which are navigating through a nav point that becomes seen will abandon their route.
	 * @param SharedSearchSpaceAgent The agent.
	 * @param RecastNavMesh The shared search space.
	 * @param NavPathPoints The points being navigated.
	 * @param OnPathCrossed The callback for when a waypoint is seen by an agent.
	 * @return Whether the nav node path is valid for the search space.
	 */
	bool SetNodesOnPaths(class USharedSearchSpaceAgent& SharedSearchSpaceAgent, const class ARecastNavMesh& RecastNavMesh, const TArray<NavNodeRef>& NavPathPoints, FPathCrossedSignature::FDelegate OnPathCrossed);
	
	bool UnsetNodesOnPaths(const class USharedSearchSpaceAgent& SharedSearchSpaceAgent, const class ARecastNavMesh& RecastNavMesh, const TArray<NavNodeRef>& NavPathPoints);

	/**
	 * @brief Adds an agent to search space.
	 * If the search space has not been registered yet, this method will add it.
	 * @param SharedSearchSpaceAgent The agent.
	 * @param RecastNavMesh The search space.
	 */
	void CreateSearchSpaces(class USharedSearchSpaceAgent& SharedSearchSpaceAgent, const class ARecastNavMesh& RecastNavMesh);

	/**
	 * @brief For a given agent, return the best possible waypoint to navigate to based on off a specific location.
	 * 
	 * @param SharedSearchSpaceAgent The agent.
	 * @param RecastNavMesh The search search space.
	 * @param CurrentLocation The specific location of the agent.
	 * @param OutSegmentProbabilities The results of computing the fitness for all waypoints.
	 * @return Whether the probabilities were computed.
	 */
	bool GetWaypointScores(const class USharedSearchSpaceAgent& SharedSearchSpaceAgent, const class ARecastNavMesh& RecastNavMesh, const FVector& CurrentLocation, TMap<NavNodeRef, FNavNodePathResult>& OutSegmentProbabilities);

	/**
	 * @brief Get a read only view of the search space info for a mesh.
	 * @param RecastNavMesh The mesh to use.
	 * @return The search space info if the mesh is being tracked.
	 */
	const FSearchSpaceInfo* GetSearchSpaceInfo(const class ARecastNavMesh& RecastNavMesh) const;
	
	bool SetFocusedSegment(const class ARecastNavMesh& RecastNavMesh, const FVector& FocusLocation);

	/**
	 * @brief Checks is a nav mesh is being treated as sa shared search space.
	 * @param RecastNavMesh The nav mesh to check.
	 * @return Whether it's being treated as a shared search space.
	 */
	bool IsTrackedNavMesh(const class ARecastNavMesh& RecastNavMesh) const;

	/**
	 * @brief Gets all the searched spaces.
	 * @param OutTrackedNavMeshes The shared search spaces.
	 */
	void GetTrackedNavMeshes(TArray<const class ARecastNavMesh*>& OutTrackedNavMeshes) const;

	/**
	 * @brief A callback on when any new shared search spaces are added.
	 * @return The callback.
	 */
	FTrackedNavMeshesUpdatedSignature& OnTrackedNavMeshesUpdated();

	/**
	 * @brief A callback on when any segment probabilities change.
	 * @return The callback.
	 */
	FSegmentProbabilitiesUpdatedSignature& OnSegmentProbabilitiesUpdated();
	
private:

	FTSTicker::FDelegateHandle TickSearchUpdateHandle;

	FTSTicker::FDelegateHandle TickGuardMinUpdateHandle;
	
	static TAutoConsoleVariable<bool> CVarDrawDebugVisualizations;

	int32 GuardMinUpdateWaypointIndex = 0;
	
	UPROPERTY()
	TMap<TObjectPtr<const class ARecastNavMesh>, FSearchSpaceInfo> SearchSpaces;

	TSharedPtr<FNavigationQueryFilter> SharedConstNavQueryFilter;

	UPROPERTY()
	bool bDrawDebugVisualizations = false;

	FTrackedNavMeshesUpdatedSignature TrackedNavMeshesUpdatedSignature;

	FSegmentProbabilitiesUpdatedSignature SegmentProbabilitiesUpdatedSignature;
	
};
