// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "..\Public\SearchSpaceLogicNavigationQueryFilter.h"
#include "SharedSearchSpaceLogicRecastQueryFilter.h"
#include "..\Public\SharedSearchSpaceSubsystem.h"

USearchSpaceLogicNavigationQueryFilter::USearchSpaceLogicNavigationQueryFilter()
{
	bIsMetaFilter = false;
	bInstantiateForQuerier = true;
}

void USearchSpaceLogicNavigationQueryFilter::InitializeFilter(const ANavigationData& NavData, const UObject* Querier,
                                                         FNavigationQueryFilter& Filter) const
{
	const ARecastNavMesh* RecastNavMesh = Cast<ARecastNavMesh>(&NavData);
	if (RecastNavMesh == nullptr)
	{
		// TODO: PANIC.
		
	}
	const USharedSearchSpaceSubsystem* SharedPatrolWaypointsSubsystem =
		Querier->GetWorld()->GetSubsystem<USharedSearchSpaceSubsystem>();
	const FSearchSpaceInfo* SearchSpaceInfo = SharedPatrolWaypointsSubsystem->GetSearchSpaceInfo(*RecastNavMesh);
	const TSharedRef<FSharedSearchSpaceLogicRecastQueryFilter> PatrolLogicRecastQueryFilter =
		MakeShared<FSharedSearchSpaceLogicRecastQueryFilter>(*SearchSpaceInfo);
	Filter.SetFilterImplementation(TSharedPtr<FSharedSearchSpaceLogicRecastQueryFilter>(PatrolLogicRecastQueryFilter).Get());
	Super::InitializeFilter(NavData, Querier, Filter);
}
