// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TalesOfUs : ModuleRules
{
	public TalesOfUs(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Tweener", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore"});
	}
}
