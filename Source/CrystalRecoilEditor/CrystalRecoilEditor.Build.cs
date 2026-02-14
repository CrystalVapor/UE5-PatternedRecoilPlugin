using UnrealBuildTool;

public class CrystalRecoilEditor : ModuleRules
{
	public CrystalRecoilEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine",
			"CrystalRecoil"
		]);

		PrivateDependencyModuleNames.AddRange([
			"InputCore",
			"Slate",
			"SlateCore",
			"UnrealEd",
			"Projects",
			"GraphEditor",
			"CurveEditor",
			"ApplicationCore"
		]);
	}
}
