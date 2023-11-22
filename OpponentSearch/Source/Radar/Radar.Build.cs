using UnrealBuildTool;

public class Radar : ModuleRules
{
    public Radar(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "BCIRadar",
                "Core",
                "DeveloperSettings",
                "GameplayTags",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
        
        PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
    }
}