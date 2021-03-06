// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BuildingEscapeEditorTarget : TargetRules
{
	public BuildingEscapeEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		bUseUnityBuild = false;
		bUsePCHFiles = false;

		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "BuildingEscape" } );
	}
}
