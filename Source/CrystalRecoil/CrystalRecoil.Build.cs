using UnrealBuildTool;

public class CrystalRecoil : ModuleRules
{
	public CrystalRecoil(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject",
			"Engine"
		]);
	}
}
