// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SharedSearchSpaceDeveloperSettings.generated.h"

/**
 * @brief DeveloperSettings for the SharedSearchSpace system.
 */
UCLASS(Config=Game)
class SHAREDSEARCHSPACE_API USharedSearchSpaceDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(USharedSearchSpaceDeveloperSettings, FEnablePatrolAISignature, bool);
	
public:
	
	/**
	 * @brief Constructor.
	 */
	USharedSearchSpaceDeveloperSettings();

	/**
	 * @brief The tick rate for updating segment info in the USharedSearchSpaceSubsystem.
	 */
	UPROPERTY(Config, EditDefaultsOnly)
	float PatrolTickRate = 0.125;
	
	UPROPERTY(Config, EditDefaultsOnly)
	bool bEnablePatrolAI = true;

	FEnablePatrolAISignature& OnEnablePatrolAI();

private:
	
	static TAutoConsoleVariable<bool> CVarEnablePatrolAI;

	FEnablePatrolAISignature OnEnablePatrolAIEvent;	
	
};
