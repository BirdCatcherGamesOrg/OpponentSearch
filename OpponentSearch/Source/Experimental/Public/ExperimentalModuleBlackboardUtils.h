#pragma once
#include "NativeBlackboardKey.h"

struct ExperimentalModuleBlackboardUtils
{
	/**
	 * @brief The BB key for the world location of an alarm stimulus.
	 */
	EXPERIMENTALMODULE_API const static TNativeObjectBlackboardKey<AActor> AlarmStimulusTarget;
	
	EXPERIMENTALMODULE_API const static TNativeBlackboardKey<UBlackboardKeyType_Vector> LastKnownAlarmStimulusLocation;
	
};
