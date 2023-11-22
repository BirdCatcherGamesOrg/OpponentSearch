// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

using UnrealBuildTool;

public class OpponentSearch : ModuleRules
{
	public OpponentSearch(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Character",
			"BCICinematicCamera",
			"BCICliques",
			"BCIInput",
			"BCIPerception",
			"BCIRadar",
			"ExperimentalModule",
			"ImGuiDashboard",
		});

		PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
	}
}
