# Patrol Algorithm

## Probability Diffusion

The foundation of the patrol algorithm is the Probability Diffusion approach in the paper. This page documents modifications to that algorithm which have improved the gameplay experience in practice.

### Total Path Consideration

The fitness function in the paper only considers the probability of the segment at the end of a path. Stale segments are almost always adjacent to each other. When only the endpoint is considered, search agents have a tendency to stale neighboring segments. This results in 2 emergent behaviors:

1. Search agents will converge from across the map to visit a small cluster of stale segments which all neighbor each other.
2. Search agents synchronize into a single file when stale segments are co-linear.
   * This behavior is very hard to break. Forcing agents to pick new points results in choosing the segment further down the line.

To mitigate this the fitness function must sum the probability of all segments along the navigation path. This biases search agents to choose long paths which traverse many stale segments instead of frequently traversed paths with only one stale segment at the very end. In addition, this causes agents to "spread out" from each other since stale paths rarely overlap.

By considering the entire path, the average staleness across all points is reduced even further and the distance between guards results in a more balanced gameplay experience.

## Gameplay Considerations

The patrol algorithm needs to be modified to handle gameplay requirements. Without these modifications, the gameplay experience is unbalanced and/or immersion breaking.

### Planned Halting

Left to their own devices, search agents can patrol indefinitely, seamlessly picking new segments to move to every time they reach their current destination. In practice, this is exhausting for players because they never get the opportunity to relax, nor get the satisfaction of disabling an opponent who has temporarily let their guard down. To mitigate this, there should be cases where guards stop moving as part of the algorithm or as random events like yawning, radioing in an update or other tropes of stealth games.

Any halts in the agents' movement must also include a sight line check so that the agent doesn't stand in place facing a wall. The agent must ray trace ahead, and if it collides with a wall, the agent should rotate a small amount until they find an open sight line.

### Short Path Filtering

Sometimes the best fit segment is located very close to the search agent. This can lead to the agent taking a single step but with a broad rotation to bring the waypoint into view. While this is correct behavior from an algorithmic standpoint, it is uncanny and breaks immersion.

Sometimes the best fit segment can be so close to the search agent that it falls inside its current movement acceptance radius so the navigation system will return synchronously. The rest of the gameplay code may not expect this, and it is easy to accidentally write callbacks which loop infinite or cause stack overflows in such cases.

To mitigate these issues, it is recommended that search agents filter out any segments which are located "too close" to the search agent. There's no hard definition, but in practice one or two times to acceptance radius of the search agent yields acceptable results.

### Corner Avoidance

Oftentimes the most stale segments are located in areas with very few open sight lines. In practice this means that search agents will move towards corners and the end of long hallways. While this is good for keeping the overall staleness of the search space down, this makes the game too easy. A guard in the corner of the map is much less likely to see the player because they lack sight lines out of these points.

To mitigate this, levels should be designed to minimize the number of corners or narrow hallways. Uninteresting corners should be removed from the ARecastNavMesh used to build the viz-mesh, or adjusted so the cost of entering a "boring" area is too high.

### 180-Degree Rotation Prevention

Stealth gameplay centers on quietly following patrollers and disabling them. If search agents are able to pivot 180 degrees, then the player may be spotted during their approach. Players feel that this is unfair, and it ruins the gameplay loop. Unfortunately, oftentimes the most stale point happens to be somewhere behind the current position of the search agent which can result in a 180-degree rotation.

To mitigate this, AI agents check if the next point on their navigation path requires a 180-degree turn. Two options for alternative patrol behavior were evaluated.

#### First Angle Filtering (Recommended)

In this approach, the search agent filters out any endpoints where the desired path begins with a 180-degree turn. If no such path exists, then the agent will halt in place for a short period of time before resuming movement. After this pause, the 180-degree restriction is lifted since the player had enough time to change their course.

#### Halt in Place (Not Recommended)

In this approach, the search agent always stops in place.

This halting behavior has several drawbacks:

1. Search agents often stand still facing the wall in front of them which breaks immersion and makes the game too easy.
2. The behavior is predictable. Whenever an agent stands still, players quickly catch on that they will rotate 180 degrees on their next move.
3. "Stutter steps" occurs when the agent's next navigation point is discovered shortly after a new point is acquired. The root cause is this:
   1. The agent reaches their current destination. The next destination requires a 180-degree turn, so they halt.
   2. The agent picks their next navigation path, which still requires a 180-degree rotation.
   3. Immediately after moving, the point that the agent chooses to move to is discovered by another agent. The agent chooses a new path, which requires another 180-degree turn, so the agent halts.
   4. Repeated many times, the agent rotates in place, taking 1 step in each direction between halts.
