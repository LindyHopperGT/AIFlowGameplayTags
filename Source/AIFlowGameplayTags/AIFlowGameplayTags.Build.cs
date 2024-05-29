// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

using UnrealBuildTool;

public class AIFlowGameplayTags : ModuleRules
{
	public AIFlowGameplayTags(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
			"AIModule",
			"BlackboardGameplayTags",
			"Flow",
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"AIFlow",
		});

	}
}
