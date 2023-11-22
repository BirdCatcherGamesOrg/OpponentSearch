// Copyright Bird Catcher Interactive, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ExperimentalModule : ModuleRules
{
	public ExperimentalModule(ReadOnlyTargetRules Target) : base(Target)
	{
		CppStandard = CppStandardVersion.Cpp20;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	    PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") } );
	    PublicDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
	    PublicDependencyModuleNames.AddRange(new string[] {
		    "AIModule", 
		    "BCIAIHelpers",
		    "Core", 
		    "CoreUObject", 
		    "DeveloperSettings", 
		    "BCICliques",
		    "BCIRadar",
		    "Engine", 
		    "BCIPerception", 
		    "GameplayTags",
		    "Navmesh", 
		    "SharedSearchSpace",
		    "SuspiciousEvent", 
		    "UMG",
    });
  }
}
