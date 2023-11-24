# BCICinematicCamera

This plugin contains code which supports UCineCameraComponents on a per-actor basis to use as the in-game camera instead of the typical spring arm mounted camera. Instead, stacks of ABoundedCameraVolumes are created. When the player moves into one of these volumes, the camera is positioned according to the constraints of the volume which may go beyond simple over-the-should points of view. Players can move between various nested volumes or into new ones and interpolate to/from positions in the new volumes.

The motivation for creating this code is to re-create the camera experience from Metal Gear Solid 1 and 2.