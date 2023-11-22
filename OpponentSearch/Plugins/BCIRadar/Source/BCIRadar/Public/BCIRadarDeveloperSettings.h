// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "BCIRadarDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class BCIRADAR_API UBCIRadarDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UBCIRadarDeveloperSettings();
	
	UPROPERTY(GlobalConfig, EditAnywhere)
	FGameplayTag WorldStaticMeshRadarVisibilityTag;

	UPROPERTY(GlobalConfig, EditAnywhere)
	FGameplayTag MovableStaticMeshRadarVisibilityTag;

};
