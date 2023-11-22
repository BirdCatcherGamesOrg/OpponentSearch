// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "BCIDynamicNamedInputSettings.h"
#include "GameFramework/InputSettings.h"

namespace
{
	void AddAxisMappings(UInputSettings& InputSettings, FDynamicAxisInputSetting& Setting)
	{
		for (FInputAxisKeyMapping& Mapping : Setting.Mappings)
		{
			Mapping.AxisName = Setting.AxisName;
			InputSettings.AddAxisMapping(Mapping);
		}
	}
}

UBCIDynamicNamedInputSettings::UBCIDynamicNamedInputSettings()
{
	CategoryName = TEXT("BCI Plugins");
	SectionName = TEXT("Dynamic Named Input");
}

void UBCIDynamicNamedInputSettings::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	UInputSettings* InputSettings = UInputSettings::GetInputSettings();

	TArray<FInputActionKeyMapping> ActionKeyMappings;
	ActionKeyMappings.Append(InputSettings->GetActionMappings());
	for (const FInputActionKeyMapping& ActionKeyMapping : ActionKeyMappings)
	{
		InputSettings->RemoveActionMapping(ActionKeyMapping, false);
	}
	
	TArray<FInputAxisKeyMapping> AxisKeyMappings;
	AxisKeyMappings.Append(InputSettings->GetAxisMappings());
	for (const FInputAxisKeyMapping& AxisKeyMapping : AxisKeyMappings)
	{
		InputSettings->RemoveAxisMapping(AxisKeyMapping, false);
	}
	
	AddAxisMappings(*InputSettings, MoveForward);
	AddAxisMappings(*InputSettings, MoveRight);

	InputSettings->ForceRebuildKeymaps();
	InputSettings->SaveKeyMappings();
}
