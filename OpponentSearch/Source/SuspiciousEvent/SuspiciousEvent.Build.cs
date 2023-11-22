using UnrealBuildTool;

public class SuspiciousEvent : ModuleRules
{
    public SuspiciousEvent(ReadOnlyTargetRules Target) : base(Target)
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
                "AudioExtensions",
                "CoreUObject",
                "DeveloperSettings",
                "Engine",
                "GameplayAbilities",
                "GameplayTags",
                "Slate",
                "SlateCore",
                "UMG",
            }
        );
        
        PublicDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
    }
}