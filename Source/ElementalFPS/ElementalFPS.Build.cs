// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ElementalFPS : ModuleRules
{
	public ElementalFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
