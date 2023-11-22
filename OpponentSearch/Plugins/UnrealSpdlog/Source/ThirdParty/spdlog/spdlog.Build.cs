// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class spdlog : ModuleRules
{
	public spdlog(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;		
		bEnableExceptions = true;
		//PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "spdlog/git/spdlog/include"));
	}
}