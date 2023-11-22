// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class BCIRadar : ModuleRules
{
	public BCIRadar(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") } );
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"DeveloperSettings",
			"Engine",
			"GameplayTags",
		});
		PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
	}
}