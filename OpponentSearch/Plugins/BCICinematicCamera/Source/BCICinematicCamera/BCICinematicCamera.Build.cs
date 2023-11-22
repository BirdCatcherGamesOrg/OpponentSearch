// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class BCICinematicCamera : ModuleRules
{
	public BCICinematicCamera(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	    PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") } );
	    PublicDependencyModuleNames.AddRange(new string[] {
		    "ActorSequence",
			"CinematicCamera",
		    "Core",
		    "CoreUObject",
		    "Engine",
		    "LevelSequence",
	    });
	    PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
   }
}
