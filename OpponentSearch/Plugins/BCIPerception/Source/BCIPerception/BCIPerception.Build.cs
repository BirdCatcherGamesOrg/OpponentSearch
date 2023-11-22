// Copyright Bird Catcher Interactive, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class BCIPerception : ModuleRules
{
	public BCIPerception(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	    PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") } );
	    PublicDependencyModuleNames.AddRange(new string[] {
		    "AIModule",
		    "Core", 
		    "CoreUObject", 
		    "Engine", 
		    "GameplayAbilities", 
		    "GameplayTasks",
		});
	    PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
  }
}
