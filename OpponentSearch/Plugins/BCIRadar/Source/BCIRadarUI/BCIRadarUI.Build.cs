// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class BCIRadarUI : ModuleRules
{
	public BCIRadarUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") } );
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"DeveloperSettings",
			"Engine",
			"GameplayAbilities",
			"GameplayTags",
			"BCIRadar",
			"UMG",
		});

		PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
	}
}