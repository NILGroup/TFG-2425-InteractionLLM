/*
* Typewriter Plugin - Unreal Engine 5 Typewriter Plugin
*
* Copyright (C) 2022 Mykhailo Oliinyk <m19tes@gmail.com> All Rights Reserved.
*/

using UnrealBuildTool;

public class TypewriterPlugin : ModuleRules
{
	public TypewriterPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				ModuleDirectory + "/Public",
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				ModuleDirectory + "/Private",
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UMG",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
