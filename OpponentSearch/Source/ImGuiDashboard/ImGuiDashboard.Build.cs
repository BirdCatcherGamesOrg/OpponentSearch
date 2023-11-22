using UnrealBuildTool;

public class ImGuiDashboard : ModuleRules
{
    public ImGuiDashboard(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "AIModule",
                "DeveloperSettings",
                "Engine",
                "GameplayAbilities",
                "ImGui",
                "InputCore",
                "NavigationSystem",
                "ProceduralMeshComponent",
                "SharedSearchSpace",
                "SuspiciousEvent",
                "ExperimentalModule",
                "UMG",
            }
        );
        PrivateDependencyModuleNames.AddRange(BCICommonUtilities.CommonDependencies());
    }
}