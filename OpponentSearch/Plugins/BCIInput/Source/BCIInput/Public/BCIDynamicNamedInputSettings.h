// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerInput.h"
#include "BCIDynamicNamedInputSettings.generated.h"

USTRUCT()
struct FDynamicAxisInputSetting
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName AxisName;

	UPROPERTY(EditDefaultsOnly)
	TArray<FInputAxisKeyMapping> Mappings;
};

/**
 * @brief Action mappings for a given action name.
 * Provides compile time access to an action.
 */
USTRUCT()
struct FDynamicActionInputSetting
{
	GENERATED_BODY()

	/**
	 * @brief The action name.
	 */
	UPROPERTY(EditDefaultsOnly)
	FName ActionName;

	/**
	 * 
	 */
	UPROPERTY(EditDefaultsOnly)
	TArray<FInputActionKeyMapping> Mappings;
};

/**
 *
 */
UCLASS(Config=Game)
class BCIINPUT_API UBCIDynamicNamedInputSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UBCIDynamicNamedInputSettings();
	
	// Movement
	UPROPERTY(GlobalConfig, EditDefaultsOnly)
	FDynamicAxisInputSetting MoveForward;

	UPROPERTY(GlobalConfig, EditDefaultsOnly)
	FDynamicAxisInputSetting MoveRight;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
};
