// (c) Kushagra, 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectDefenderTarget : TargetRules
{
	public ProjectDefenderTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ProjectDefender" } );
	}
}
