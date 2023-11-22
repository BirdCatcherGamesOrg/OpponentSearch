// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class BCICliques : ModuleRules
{
	public BCICliques(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	    PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") } );
	    PublicDependencyModuleNames.AddRange(new string[] {
	      "Core",
	      "CoreUObject",
	      "Engine",
	      "NavigationSystem",
	    });
	    PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
  }
}
