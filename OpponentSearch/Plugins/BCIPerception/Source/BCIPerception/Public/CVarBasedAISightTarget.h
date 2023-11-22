// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISightTargetInterface.h"
#include "UObject/Object.h"
#include "CVarBasedAISightTarget.generated.h"

/**
 * @brief an AISightTargetInterface which can be used to override any visibility settings to be invisible.
 */
UCLASS()
class BCIPERCEPTION_API UCVarBasedAISightTarget : public UObject, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	
	/**
	 * @copydoc IAISightTargetInterface::CanBeSeenFrom
	 */
	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate) override;

private:

	static TAutoConsoleVariable<bool> CVarEnableVisibility;
	
};
