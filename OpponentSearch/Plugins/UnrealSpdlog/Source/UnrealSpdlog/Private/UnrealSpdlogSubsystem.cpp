// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "UnrealSpdlogSubsystem.h"
#include "SpdlogOutputDevice.h"

void UUnrealSpdlogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UUnrealSpdlogSubsystem::OUTPUTDEVICE = MakeShared<FSpdlogOutputDevice>();
	Super::Initialize(Collection);
	GLog->AddOutputDevice(OUTPUTDEVICE.Get());
	OUTPUTDEVICE->MakeSpdDefault();
}

void UUnrealSpdlogSubsystem::Deinitialize()
{
	Super::Deinitialize();
	GLog->Flush();
	GLog->RemoveOutputDevice(OUTPUTDEVICE.Get());
	OUTPUTDEVICE->Flush();
}
