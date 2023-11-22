// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RadarUISettings.generated.h"

USTRUCT()
struct FPerceivedState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayTag;

	UPROPERTY(EditAnywhere)
	FText Text;
};

/**
 * 
 */
UCLASS(Config=Game)
class BCIRADARUI_API URadarUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	URadarUISettings();
	
	UPROPERTY(Config, EditAnywhere)
	FPerceivedState Alert;

	UPROPERTY(Config, EditAnywhere)
	FPerceivedState Caution;
	
};
