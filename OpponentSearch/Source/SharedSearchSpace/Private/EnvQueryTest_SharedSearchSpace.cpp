// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "EnvQueryTest_SharedSearchSpace.h"

#include "SharedSearchSpaceAgent.h"
#include "SharedSearchSpaceBlackboardUtils.h"
#include "SharedSearchSpaceSubsystem.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

UEnvQueryTest_SharedSearchSpace::UEnvQueryTest_SharedSearchSpace()
{
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
}

void UEnvQueryTest_SharedSearchSpace::RunTest(FEnvQueryInstance& QueryInstance) const
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


	const AActor* Owner = Cast<AActor>(QueryInstance.Owner);
	const USharedSearchSpaceAgent* PatrollerSightBox = Owner->FindComponentByClass<USharedSearchSpaceAgent>();
	TMap<NavNodeRef, FNavNodePathResult> SegmentProbabilities;
	SharedSearchSpaceSubsystem->GetWaypointScores(*PatrollerSightBox, *RecastNavMesh, Owner->GetActorLocation(), SegmentProbabilities);
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const UEnvQueryItemType_Point* PointCDO = GetDefault<UEnvQueryItemType_Point>();
		const FNavLocation NavLocation = PointCDO->GetItemNavLocation(QueryInstance.RawData.GetData() + QueryInstance.Items[It.GetIndex()].DataOffset);
		const FNavNodePathResult SegmentProbability = SegmentProbabilities.FindRef(NavLocation.NodeRef);
		double Fitness = SegmentProbability.Fitness;
		if (SegmentProbability.PathFindingResult.Path != nullptr)
		{
			// Case is the next node is a direct continuation of the current path.
			if (PatrollerSightBox->GetOwner()->GetActorForwardVector().Dot(SegmentProbability.PathFindingResult.Path->GetSegmentDirection(0)) <= 0.2)
			{
				Fitness /= 1000000;
			}
		}
		It.SetScore(TestPurpose, FilterType, Fitness, 0, TNumericLimits<float>::Max());
	}

	
}

FText UEnvQueryTest_SharedSearchSpace::GetDescriptionTitle() const
{
	return NSLOCTEXT(UE_MODULE_NAME, "SharedSearchSpaceTestTitle", "Shared Search Space Test: Use Clique to find highest probability point.");
}

FText UEnvQueryTest_SharedSearchSpace::GetDescriptionDetails() const
{
	return NSLOCTEXT(UE_MODULE_NAME, "SharedSearchSpaceTestDescription", "Use Clique to find highest probability point.");
}
