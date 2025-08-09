// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MenuSystem_EOS_Steam : ModuleRules
{
	public MenuSystem_EOS_Steam(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
