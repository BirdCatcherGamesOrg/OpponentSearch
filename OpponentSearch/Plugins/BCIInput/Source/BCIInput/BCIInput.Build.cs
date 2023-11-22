// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class BCIInput : ModuleRules
{
	public BCIInput(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") } );
	    PublicDependencyModuleNames.AddRange(new string[] {
		    "Core", 
		    "CoreUObject", 
		    "DeveloperSettings",
		    "Engine",
		    "InputCore", 
		    "HeadMountedDisplay",
		    "Slate",
		    "UMG",
	    });
	    PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
  }
}
