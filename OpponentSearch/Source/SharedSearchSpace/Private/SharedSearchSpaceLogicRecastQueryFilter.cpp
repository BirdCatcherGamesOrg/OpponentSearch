// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "SharedSearchSpaceLogicRecastQueryFilter.h"
#include "SharedSearchSpaceSubsystem.h"

FSharedSearchSpaceLogicRecastQueryFilter::FSharedSearchSpaceLogicRecastQueryFilter(const FSearchSpaceInfo& InSearchSpaceInfo, double inmult)
{
	isVirtual = true;
	mult = inmult;
	for (const auto& NavNode : InSearchSpaceInfo.NavNodes)
	{
		SegmentProbabilities.Add(NavNode.Key, NavNode.Value.Probability / (2 * (1 + NavNode.Value.SegmentPatrollers.Num())));
	}
}

INavigationQueryFilterInterface* FSharedSearchSpaceLogicRecastQueryFilter::CreateCopy() const
{
	return new FSharedSearchSpaceLogicRecastQueryFilter(*this);
}

dtReal FSharedSearchSpaceLogicRecastQueryFilter::getVirtualCost(const dtReal* pa, const dtReal* pb,
	const dtPolyRef prevRef, const dtMeshTile* prevTile, const dtPoly* prevPoly, const dtPolyRef curRef,
	const dtMeshTile* curTile, const dtPoly* curPoly, const dtPolyRef nextRef, const dtMeshTile* nextTile,
	const dtPoly* nextPoly) const
{
	double Curr = *SegmentProbabilities.Find(curRef);
	const float* PrevProbability = SegmentProbabilities.Find(prevRef);
	if (PrevProbability)
	{
		Curr += *PrevProbability;
		Curr /= 2;
	}
	return FMath::Max(1, 1000 + (mult * Curr));
}
