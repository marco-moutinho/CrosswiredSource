// Copyright Epic Games, Inc. All Rights Reserved.

#include "MP_NPC.h"

#define LOCTEXT_NAMESPACE "FMP_NPCModule"

void FMP_NPCModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FMP_NPCModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMP_NPCModule, MP_NPC)