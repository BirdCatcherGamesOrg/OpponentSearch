// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;
public class UnrealSpdlog : ModuleRules
{
	public UnrealSpdlog(ReadOnlyTargetRules Target) : base(Target)
	{		
	
	    bEnableExceptions = true;
		PublicIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModuleDirectory, "../ThirdParty/spdlog/git/spdlog/include")
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"spdlog",
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
			}
		);
	}
}