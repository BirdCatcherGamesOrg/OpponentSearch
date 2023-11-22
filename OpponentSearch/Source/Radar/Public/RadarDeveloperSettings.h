// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BCIRadarDeveloperSettings.h"
#include "RadarDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class RADAR_API URadarDeveloperSettings : public UBCIRadarDeveloperSettings
{
	GENERATED_BODY()

public:

	URadarDeveloperSettings();

	UPROPERTY(Config, EditDefaultsOnly)
	bool bEnableRadar;

};
