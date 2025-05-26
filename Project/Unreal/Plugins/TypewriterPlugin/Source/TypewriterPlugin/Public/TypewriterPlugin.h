/*
* Typewriter Plugin - Unreal Engine 5 Typewriter Plugin
*
* Copyright (C) 2022 Mykhailo Oliinyk <m19tes@gmail.com> All Rights Reserved.
*/

#pragma once

#include "Modules/ModuleManager.h"

class FTypewriterPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
