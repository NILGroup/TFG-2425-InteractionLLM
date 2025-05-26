/*
* Typewriter Plugin - Unreal Engine 5 Typewriter Plugin
*
* Copyright (C) 2022 Mykhailo Oliinyk <m19tes@gmail.com> All Rights Reserved.
*/

#include "TypewriterPlugin.h"

#define LOCTEXT_NAMESPACE "FTypewriterPluginModule"

void FTypewriterPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FTypewriterPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTypewriterPluginModule, TypewriterPlugin)