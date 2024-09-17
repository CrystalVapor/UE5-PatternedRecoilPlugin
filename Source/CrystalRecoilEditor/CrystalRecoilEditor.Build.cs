using UnrealBuildTool;

public class CrystalRecoilEditor : ModuleRules
{
    public CrystalRecoilEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "InputCore",
                "AssetTools",
                "CrystalRecoil",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "UnrealEd", 
                "CrystalRecoil",
                "PropertyEditor",
                "AssetTools",
                "Projects",
                "EditorInteractiveToolsFramework",
                "GraphEditor", 
                "CurveEditor",
                "ApplicationCore",
            }
        );
    }
}