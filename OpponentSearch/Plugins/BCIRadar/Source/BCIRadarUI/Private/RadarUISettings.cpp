// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "RadarUISettings.h"
#include "NativeGameplayTags.h"
#include "NativeGameplayTagUtilities.h"

namespace
{
	DEFINE_TAG_STATIC(URadarUISettings, AlertStatusTag);
	DEFINE_TAG_STATIC(URadarUISettings, CautionStatusTag);
}

URadarUISettings::URadarUISettings()
{
	Alert.GameplayTag = AlertStatusTag;
	Caution.GameplayTag = CautionStatusTag;
}
