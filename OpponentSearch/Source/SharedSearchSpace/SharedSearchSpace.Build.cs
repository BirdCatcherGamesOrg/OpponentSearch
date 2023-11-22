using UnrealBuildTool;

public class SharedSearchSpace : ModuleRules
{
    public SharedSearchSpace(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AIModule",
                "BCIAIHelpers",
                "BCICliques",
                "CoreUObject",
                "DeveloperSettings",
                "Engine",
                "NavigationSystem",
                "Navmesh",
                "Slate",
                "SlateCore",
            }
        );
        
        PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
    }
}