// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DebugDashboardConstants.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class IMGUIDASHBOARD_API UDebugDashboardConstants : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<class UFloatingTextWidget> FloatingTextWidgetType;
	
};
