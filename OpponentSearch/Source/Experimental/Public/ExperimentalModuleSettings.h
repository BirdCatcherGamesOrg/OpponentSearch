// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ExperimentalModuleSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class EXPERIMENTALMODULE_API UExperimentalModuleSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UExperimentalModuleSettings();

	UPROPERTY(Config, EditDefaultsOnly)
	FName AlarmingStimulusTag;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> SightConeRadarMaterial;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<class UMaterialInterface> EnemyBlinkingRadarMaterial;

	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<class UBlackboardData> SimpleAttackBlackboardData;
};
