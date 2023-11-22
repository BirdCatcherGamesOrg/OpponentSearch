// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "SuspiciousEventSettings.h"
#include "NativeGameplayTags.h"
#include "NativeGameplayTagUtilities.h"

DEFINE_TAG_STATIC(USuspiciousEventSettings, ReactingToSuspiciousEventWidgetStatusTag);

USuspiciousEventSettings::USuspiciousEventSettings()
{
	SuspiciousStimulusTag = TEXT("SuspiciousStimulus");
	ReactingToSuspiciousEventWidgetStatus = ReactingToSuspiciousEventWidgetStatusTag;
}
