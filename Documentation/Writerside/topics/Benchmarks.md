# Benchmarks

This page covers the performance metrics of the algorithm at various scales and with a handful of optimizations enabled or disabled.

## Invariants

### Hardware

AMD Ryzen 9 3900XT 12-Core Processor @ 3.79 GHz with 64 GB RAM

### Development Editor Target

The game is compiled to run the Unreal editor and all testing was done using the Play in Editor mode.

### Un-tuned Segment Sizes

All nav meshes were constructed with default parameters.

## Performance Analysis

Stats are provided by the Unreal Insights tool. The goal of this analysis is to determine whether the needs of a game's design can be met within the upper bound across a variety of map sizes. The design question being asked is "For a given level size, what is the maximum number of guards that can patrol at once and hold a mostly steady 60fps?"

The baseline of 3 is always present, so that this implementation can be compared with the performance in the original paper. The upper bound is determined by the number of guards, discovered through experimentation.

### No Optimizations

| Map Area (m<sup>2</sup>) | Number of Segments | Test Category | Number of Search Agents | Decision Time (ms) | Worst FPS |
|--------------------------|--------------------|---------------|-------------------------|--------------------|-----------|
| 256                      | 202                | Baseline      | 3                       | 1.39               | 60        |
| 256                      | 202                | Upper Bound   | 15                      | 18.23              | 58        |
| 1000                     | 327                | Baseline      | 3                       | 9.7                | 60        |
| 1000                     | 327                | Upper Bound   | 5                       | 18.22              | 58        |
| 4,000                    | 716                | Baseline      | 3                       | 61.10              | 55        |            

With no optimizations, the smallest level performs well up to 15 guards. At this point, any time a guard makes a decision, which is roughly every 10-15 seconds or so, framerate dips are guaranteed.

The medium level's baseline is acceptable, but very quickly has performance issues where even a small number of guards leads to dropped frames whenever a guard picks a direction.

The large level has framerate issues even at baseline. With this small number of guards the number of times a decision is made is infrequent, but it is a steep cost of 4-5 frames.

### With Distance Heuristic

| Map Area (m<sup>2</sup>) | Number of Segments | Test Category   | Number of Search Agents | Decision Time (ms) | Worst FPS |
|--------------------------|--------------------|-----------------|-------------------------|--------------------|-----------|
| 256                      | 202                | Baseline        | 3                       | 1.25               | 60        |
| 256                      | 202                | Old Upper Bound | 15                      | 4.13               | 60        |   
| 256                      | 202                | New Upper Bound | 45 (3x increase)        | 14.24              | 60        |     
| 1000                     | 327                | Baseline        | 3                       | 6.78               | 60        | 
| 1000                     | 327                | Old Upper Bound | 5                       | 11.09              | 60        |  
| 1000                     | 327                | New Upper Bound | 11 (2.25x increase)     | 15.02              | 59        |     
| 4,000                    | 716                | Baseline        | 3                       | 57.57              | 55        |

As expected, the baseline performance for each level doesn't change much with the distance heuristic.

The upper bound for the small level is now 45 guards, which is well above any typical gameplay design's needs.

The medium level can now support 11 guards without framerate issues, which is above typical designs but potentially below particularly guard heavy sequences.

The large level is still not viable without framerate issues.

### With Cached Path Amortization

Keep in mind that the decision time is amortized. The correct way to interpret this is "how much time per frame is spent on pathfinding."

| Map Area (m<sup>2</sup>) | Number of Segments | Test Category | Number of Search Agents | Decision Time (ms) | Worst FPS | Amortization Time Window (s) |
|--------------------------|--------------------|---------------|-------------------------|--------------------|-----------|------------------------------|
| 256                      | 202                | Baseline      | 3                       | 0.01               | 60        | 1                            |
| 256                      | 202                | Upper Bound   | 109                     | 15.68              | 60        | 10                           |
| 1000                     | 327                | Baseline      | 3                       | 0.06               | 60        | 1                            |
| 1000                     | 327                | Upper Bound   | 98                      | 15.32              | 60        | 10                           |
| 4,000                    | 716                | Baseline      | 3                       | 0.51               | 60        | 1                            |
| 4,000                    | 716                | Upper Bound   | 50                      | 15.81              | 60        | 10                           |


As expected, the amortized baseline performance for each level improves by 2 orders of magnitude with even a short window of 1s.

All levels can now support more guards than would ever be used by game designs and hold a steady 60fps, but at the cost of a constant framerate overhead. At these upper bounds, in practice the only thing the game can do is run pathfinding.

### With Max Distance Heuristic

The max distance heuristic is still experimental, but it shows promise. Setting the max distance to 40m allowed 15 guards to patrol the 4000m<sup>2</sup> map at 60 fps without amortization. With amortization, 75 guards were able to patrol the 4000m<sup>2</sup> map, albeit with inconsistent FPS ranging from 40-60 the whole time. For the 256 m<sup>2</sup> map and a max distance of 16m, the decision time for 3 guards was reduced to 0.52ms with no notable change to guard behavior.

## Observations
 
1. The implementation broadly matches the benchmarks provided in the paper
2. The distance heuristic check is essentially free and required to scale up for medium-sized maps
3. Large maps require cached path amortization and/or the max distance heuristic

## Conclusion

The algorithm is performant for the vast majority of gameplay purposes. Observing popular games with optional or required stealth mechanics, there are very few situations where the design calls for more than 5 guards on patrol in a level. Sequences with many enemies typically have a mixture of guards patrolling or standing still doing some miscellaneous activity. There are many options for tuning the algorithm for performance, the majority haven't been explored yet by this project. Gameplay engineers should feel confident that they can use this algorithm in their games.