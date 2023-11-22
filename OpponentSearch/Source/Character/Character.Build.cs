// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Character : ModuleRules
{
	public Character(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;
	    PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") } );
	    PublicDependencyModuleNames.AddRange(new string[] {
		    "AIModule",
		    "BCICinematicCamera",
		    "BCIInput",
		    "BCIPerception",
		    "BCIRadar",
		    "BCIRadarUI",
		    "Core", 
		    "CoreUObject",
		    "DeveloperSettings",
		    "Engine",
		    "GameplayTags",
		    "InputCore", 
		    "Radar",
		    "UMG",
	    });
	    PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
  }
}
