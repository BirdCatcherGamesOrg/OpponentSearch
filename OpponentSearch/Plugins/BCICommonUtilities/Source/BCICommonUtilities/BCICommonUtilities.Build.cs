// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

using System.Collections.Generic;
using UnrealBuildTool;

public class BCICommonUtilities : ModuleRules
{
	public BCICommonUtilities(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"GameplayAbilities",
			});
		PublicDependencyModuleNames.AddRange(CommonDependencies());
		PublicDependencyModuleNames.Remove("BCICommonUtilities");
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
			});
		
		PublicDefinitions.Add("USE_UNREAL_SPDLOG=1");
	}

	public static IEnumerable<string> CommonDependencies()
	{
		return new[]
		{
			"BCICommonUtilities",
			"JsonUtilities",
			"UnrealSpdlog",
		};
	}
}
