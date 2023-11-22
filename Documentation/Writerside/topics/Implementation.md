# Implementation

The propagation algorithm is implemented in a set of core C++ classes with different search agents that integrate with that core. The core classes handle segment information and computes the fitness function over all segments for a search agent on demand to return the nav path that an agent should use. The search agents use the path

## Core Classes

### UCliqueHandler

The BCIClique Plu

### Search Agent

### USharedSearchSpaceSubsystem

The subsystem which handles all operations over segments for a clique of search agents over an ARecastNavMesh. The lifecycle of the subsystem is scoped to a UWorld, which matches the lifecycle of an ARecastNavMesh. Search agents register with a specific ARecastNavMesh, and all agents are grouped into a set based on their chosen mesh. The subsystem ticks at a given rate and this tick function runs the propagation and check for discovery steps. For performance reasons, this function doesn't tick on every frame.

This subsystem is the source of truth on segment probabilities. A search agent looking for a

### FSharedSearchSpaceLogicRecastQueryFilter

This struct computes the cost of traversing a recast navmesh poly using a snapshot of the probability. The filter is a virtual filter which runs at a very frequent rate, so all costs are cached instead of updating live.

This filter is used in 2 ways:

1. Sum all probabilities for every segment on a path
2. Compute the cost of moving along a path with the detour algorithm

### Detection

All Search Agents approximate a sight cone with a Box. While cones are possible, approximating with a box is simple. Once a point is culled with the box, we line trace.  

## Search Agents Implementations

There are 

### Native C++

The native C++ implementation search agent uses no blueprints or in-editor workflows. Instead, it directly uses navigation components and the USharedSearchSpaceSubsystem to dispatch AIMove requests with callbacks to implement the patrol gameplay behavior.

Pro:
* Only requires C++ knowledge
* Simple for gameplay engineers to use and modify
* Directly uses the navigation system for faster performance
* Callbacks provide flexible hooks for other classes

Con:
* Doesn't use the managed Behavior Tree AI approach
* Gameplay AI specialists may not be comfortable with C++
* Difficult to implement complicated behavior with callbacks

#### UNativeSharedSearchSpaceLogicComponent

This component is responsible for calling functions on the USharedSearchSpaceSubsystem and AIController components to managed move requests. A single move request may traverse anywhere from 0 to an uncapped number of segments depending on the state of the segments and the pathfinding outcomes. Requests are async and execute on the next frame. This is because the fitness function may determine that the best waypoint is located within the acceptance radius of the move request, which will cause the navigation system to immediately succeed. Improperly handling this case results in infinite loops or stack overflow as callbacks spin on requesting to move to the same point forever.

When an agent successfully reaches a target segment, they will attempt to find another segment to move to. This movement is subject to the 180 degree constraint, and if the movement requires a 180-degree rotation then the request terminates. Otherwise, the agent will immediately take the next path without stopping. This process can repeat indefinitely, so an unobstructed search agent can navigate the space without ever stopping until the navigation is explicitly halted by another mechanism.

#### UPatrolSharedSearchSpaceComponent

This component wraps the UNativeSharedSearchSpaceLogicComponent into a "patrol" behavior for the search agent. Currently, all this does is halt the agent in place for a few seconds when the 180-degree rotation case occurs before resuming movement, but conceivably this component could be expanded to include gameplay cues like animating the guards yawning, stretching their limbs, pausing to look at their watch, or myriad other behaviors for gameplay or immersion purposes.

### Behavior Tree

The Behavior Tree search agent implementation uses the managed Unreal AI features:

* Behavior Trees
* Blackboards
* Environment Query System (EQS)

This approach is suitable for production use of the Shared Search Space navigation system. By integrating with Unreal's AI system, search behavior becomes a subcomponent of an agent's AI implementation.

Pros:
* Integrates with the managed AI framework
* Simple for gameplay AI engineers to use
* Behavior can be modified without C++
* Implementation is split into smaller re-usable task and EQS nodes

Cons:
* Slower than a native C++ implementation
* C++ engineers may not be familiar with Behavior Trees
* Functional testing Behavior Trees is difficult

#### UEnvQueryGenerator_SharedSearchSpaceNodes

Query to get all nav points on an ARecastNavMesh, then score them using the fitness function.

#### UEnvQueryTest_SharedSearchSpace

#### UBTTask_SharedSearchSpaceMove
