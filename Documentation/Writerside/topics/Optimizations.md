# Optimizations

The bottleneck for this algorithm is calculating the minimum path length for all guards. 

## Supported

### Guard Min Distance Heuristic

This optimization adds a fast heuristic before calculating the path length for a guard such that only guards with a Euclidean distance lower than the current min will be considered.

### Cached Path Amortization

This optimization computes the guard min for a subset of segments every frame over a configurable time window. When a search agent needs to determine its next segment, the path will be pulled from the cache. This approach has two drawbacks:

1. A fixed amount of CPU time every frame will be spent on computing guard mins
2. Long time windows compute suboptimal paths

The impact to path accuracy varies with the move speed of the search agents, the length of the window and the size of segments. For example, a 10s window with guards moving 2mps will be inaccurate by as much as 20 meters. This usually spans 2-3 segments which is enough to have measurable impact to the average staleness of segments and degrades guard behavior by having them group up, rotate 180 degrees and more.

In practice, when there are a large number of guards on a large map, the impact to gameplay is insignificant because there is enough space for guards to walk past each other and pick far apart segments. 

### Max Distance Heuristic (Not Fully Implemented)

This optimization adds a fast heuristic before calculating the fitness function for a given segment where the guard min for a segment is only computed if the Euclidean distance between the requester and the segment is lower than some threshold. This optimization should allow guards to run on large maps with little change in the observed behavior. Note that on small maps, this heuristic adds little benefit but can cause a dramatic regression in guard behavior. This optimization is a candidate for 1st class support in this project, but more work needs to be done to finetune the cutoff distance and provide an objective evaluation of the behavioral impact to guard patrol routes.

### 180 Degree Pruning (Not Implemented)

This optimization would be to do partial paths for a short distance and only pick the start of paths which don't require a 180-degree turn. This change is incompatible with cached path amortization, so it wasn't explored yet.
 
### Cached Path with TTL (Not Implemented)

This optimization caches the paths for a short period of time from previous fitness function computations. This should yield similar results as the amortized optimization, and with similar drawbacks, but without the overhead of running pathfinding on every single frame. 

## Unsupported

These are identified optimizations that are recommended not to do.

### Async Pathfinding

It is highly unlikely that Unreal will ever support async pathfinding for myriad reasons. Community supported async pathfinding implementations exist, but it's unclear how well they integrate with Unreal's native navigation system, libraries built on top of that, or stability on consoles. Therefore, async path finding is not supported by this project.

### Custom A-Star

The pathfinding algorithm used in Unreal is the "detour" algorithm which is significantly more computationally expensive than a basic a-star algorithm.

Not recommended as this loses compatibility with all UE5 tools and lack of documentation in engine/online.