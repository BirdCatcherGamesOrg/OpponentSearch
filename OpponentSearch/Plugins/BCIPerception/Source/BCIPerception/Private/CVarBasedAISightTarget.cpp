// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "CVarBasedAISightTarget.h"
#include "ConsoleVariableUtilities.h"

TAutoConsoleVariable<bool> UCVarBasedAISightTarget::CVarEnableVisibility(
	AUTO_CVAR_NAME("EnableVisibility"),
	true,
	TEXT(""));

UAISense_Sight::EVisibilityResult UCVarBasedAISightTarget::CanBeSeenFrom(const FCanBeSeenFromContext& Context,
	FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested,
	float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	if (!CVarEnableVisibility.GetValueOnAnyThread())
	{
		OutSightStrength = TNumericLimits<float>::Lowest();
		return UAISense_Sight::EVisibilityResult::NotVisible;
	}
	OutSightStrength = 1.0;
	return UAISense_Sight::EVisibilityResult::Visible;
}
