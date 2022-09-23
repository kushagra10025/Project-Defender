// (c) Kushagra, 2022

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectDefenderEditorTarget : TargetRules
{
	public ProjectDefenderEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ProjectDefender" } );
	}
}
