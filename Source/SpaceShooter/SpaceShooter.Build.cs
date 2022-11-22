// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpaceShooter : ModuleRules
{
	public SpaceShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "UMG", "HTTP", "BCClientPlugin", "OnlineSubsystem", "OnlineSubsystemUtils", "Networking", "HTTP", "Sockets" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "Json" });
        
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemBrainCloud");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
