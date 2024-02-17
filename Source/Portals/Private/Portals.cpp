// Copyright Epic Games, Inc. All Rights Reserved.

#include "Portals.h"

#define LOCTEXT_NAMESPACE "FPortalsModule"

void FPortalsModule::StartupModule()
{
	const FString ShaderDirectory = FPaths::Combine(FPaths::ProjectPluginsDir(), "Unreal-Engine-Portals", TEXT("Shaders"));
	AddShaderSourceDirectoryMapping("/Portals", ShaderDirectory);
}

void FPortalsModule::ShutdownModule()
{
	ResetAllShaderSourceDirectoryMappings();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPortalsModule, Portals)