// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpaceShooterTarget : TargetRules
{
	public SpaceShooterTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "SpaceShooter" } );
        
        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            // use this compiler flag for Xcode 14 (Apple clang version 14.0.0)
            // commandline option to avoid errors in Unreal source
            // not available in Xcode 13.2 or less (Apple clang version 13.0.0)
            bOverrideBuildEnvironment = true;
            AdditionalCompilerArguments = "-Wno-unused-but-set-variable";
        }
	}
}
