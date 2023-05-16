using UnrealBuildTool;
using System.Collections.Generic;

public class ArcaneWarriorsTarget : TargetRules
{
	public ArcaneWarriorsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ArcaneWarriors" } );
	}
}
