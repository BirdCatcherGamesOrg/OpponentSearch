// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "EnvQueryGenerator_SharedSearchSpaceNodes.h"
#include "SharedSearchSpaceBlackboardUtils.h"
#include "SharedSearchSpaceSubsystem.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

UEnvQueryGenerator_SharedSearchSpaceNodes::UEnvQueryGenerator_SharedSearchSpaceNodes()
{
	ItemType = UEnvQueryItemType_Point::StaticClass();
}

void UEnvQueryGenerator_SharedSearchSpaceNodes::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	USharedSearchSpaceSubsystem* SharedSearchSpaceSubsystem = UWorld::GetSubsystem<USharedSearchSpaceSubsystem>(QueryInstance.World);
	if (SharedSearchSpaceSubsystem == nullptr)
	{
		return;
	}
	const ARecastNavMesh* RecastNavMesh = SharedSearchSpaceBlackboardUtils::GetRecastNavMesh(*SharedSearchSpaceSubsystem , QueryInstance);
	if (RecastNavMesh == nullptr)
	{
		return;
	}
	
	const FSearchSpaceInfo* SearchSpaceInfo = SharedSearchSpaceSubsystem->GetSearchSpaceInfo(*RecastNavMesh);
	for (const auto& NavNode : SearchSpaceInfo->NavNodes)
	{
		FNavLocation Location;
		Location.NodeRef = NavNode.Key;
		RecastNavMesh->GetPolyCenter(NavNode.Key, Location.Location);
		QueryInstance.AddItemData<UEnvQueryItemType_Point>(Location);
	}
}

FText UEnvQueryGenerator_SharedSearchSpaceNodes::GetDescriptionTitle() const
{
	return NSLOCTEXT(UE_MODULE_NAME, "SharedSearchSpaceNodesGeneratorTitle", "Shared Search Space: generate set of points from shared search spaces");
}

FText UEnvQueryGenerator_SharedSearchSpaceNodes::GetDescriptionDetails() const
{
	return NSLOCTEXT(UE_MODULE_NAME, "SharedSearchSpaceNodesGeneratorDescription", "Uses Clique of Querier Actor");
}
