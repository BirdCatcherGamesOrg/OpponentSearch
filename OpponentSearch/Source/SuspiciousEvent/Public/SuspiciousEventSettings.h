// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "SuspiciousEventSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class SUSPICIOUSEVENT_API USuspiciousEventSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	USuspiciousEventSettings();

	UPROPERTY(Config, EditDefaultsOnly)
	FName SuspiciousStimulusTag;

	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag ReactingToSuspiciousEventWidgetStatus;

	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag TemporaryReactingToAlarmEventWidgetStatus;
	
	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<class USuspiciousSoundPerceivedStatusWidget> DefaultSuspiciousSoundPerceivedStatusWidget;
	
};
