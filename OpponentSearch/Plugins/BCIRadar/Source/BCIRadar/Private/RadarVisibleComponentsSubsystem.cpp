// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "RadarVisibleComponentsSubsystem.h"
#include "BCIRadarLogCategory.h"
#include "XLog.h"

void URadarVisibleComponentsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	LOG_INITIALIZE_SUBSYSTEM(LogBCIRadar);
	Super::Initialize(Collection);
}

const TMultiMap<FGameplayTag, TObjectPtr<UPrimitiveComponent>>& URadarVisibleComponentsSubsystem::GetVisibleComponentsByGroup()
{
	return VisibleComponentsByGroup;
}

void URadarVisibleComponentsSubsystem::RegisterComponent(const FGameplayTag& Group, UPrimitiveComponent& PrimitiveComponent)
{
	VisibleComponentsByGroup.Add(Group, &PrimitiveComponent);
}
