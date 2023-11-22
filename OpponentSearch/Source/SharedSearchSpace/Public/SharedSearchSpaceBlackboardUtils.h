#pragma once

#include "NativeBlackboardKey.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

/**
 * @brief Utils methods and properties for Blackboards to integrate with the SharedSearchSpace system.
 */
struct SharedSearchSpaceBlackboardUtils
{
	
	/**
	 * @brief Get the RecastNavMesh from the querying AActor's clique.
	 * @param SharedSearchSpaceSubsystem The subsystem instance to use.
	 * @param QueryInstance The query instance.
	 * @return The Navmesh, if it exists.
	 */
	static const class ARecastNavMesh* GetRecastNavMesh(class USharedSearchSpaceSubsystem& SharedSearchSpaceSubsystem, FEnvQueryInstance& QueryInstance);

	/**
	 * @brief The BB key for a UCliqueHandler component.
	 */
	SHAREDSEARCHSPACE_API const static TNativeObjectBlackboardKey<class UCliqueHandler> CliqueHandlerKey;

	/**
	 * @brief The BB key for a UPatrollerSightBox.
	 */
	SHAREDSEARCHSPACE_API const static TNativeObjectBlackboardKey<class USharedSearchSpaceAgent> SharedSearchSpaceAgentKey;

	/**
	 * @brief The BB key for the initial angle from the forward vector to the start of the navigation corridor.
	 */
	SHAREDSEARCHSPACE_API const static TNativeBlackboardKey<class UBlackboardKeyType_Float> MoveToInitialAngleKey;

	/**
	 * @brief The BB key for the location to AIMoveTo.
	 */
	SHAREDSEARCHSPACE_API const static TNativeBlackboardKey<class UBlackboardKeyType_Vector> MoveToLocation;
	
};
