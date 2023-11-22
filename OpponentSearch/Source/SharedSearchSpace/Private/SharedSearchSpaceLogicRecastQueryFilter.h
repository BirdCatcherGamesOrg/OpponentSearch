// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SharedSearchSpaceSubsystem.h"
#include "Detour/DetourLargeWorldCoordinates.h"
#include "NavMesh/RecastQueryFilter.h"

/**
 * 
 */
class FSharedSearchSpaceLogicRecastQueryFilter : public FRecastQueryFilter
{
 
public:

	FSharedSearchSpaceLogicRecastQueryFilter(const FSearchSpaceInfo& InSearchSpaceInfo, double inmult = -1);

	virtual INavigationQueryFilterInterface* CreateCopy() const override;

	virtual dtReal getVirtualCost(const dtReal* pa, const dtReal* pb, const dtPolyRef prevRef, const dtMeshTile* prevTile, const dtPoly* prevPoly, const dtPolyRef curRef, const dtMeshTile* curTile, const dtPoly* curPoly, const dtPolyRef nextRef, const dtMeshTile* nextTile, const dtPoly* nextPoly) const override;

private:
	
	//FSearchSpaceInfo SearchSpaceInfo;

	TMap<dtPolyRef, float> SegmentProbabilities;

	double mult = -1;
};
