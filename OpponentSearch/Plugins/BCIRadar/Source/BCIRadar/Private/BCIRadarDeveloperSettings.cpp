#include "BCIRadarDeveloperSettings.h"
#include "DeveloperSettingsUtilities.h"
#include "NativeGameplayTags.h"
#include "NativeGameplayTagUtilities.h"

namespace
{
	DEFINE_TAG_STATIC(UBCIRadarDeveloperSettings, WorldStaticMeshRadarVisiblity);
	DEFINE_TAG_STATIC(UBCIRadarDeveloperSettings, MovableStaticMeshRadarVisibility);
}

UBCIRadarDeveloperSettings::UBCIRadarDeveloperSettings()
{
	CategoryName = DeveloperSettingsUtilities::CategoryName;
	SectionName = TEXT("Radar");
	WorldStaticMeshRadarVisibilityTag = WorldStaticMeshRadarVisiblity;
	MovableStaticMeshRadarVisibilityTag = MovableStaticMeshRadarVisibility;
}
