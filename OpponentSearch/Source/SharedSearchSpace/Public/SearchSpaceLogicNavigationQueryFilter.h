// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavFilter_AIControllerDefault.h"
#include "UObject/Object.h"
#include "SearchSpaceLogicNavigationQueryFilter.generated.h"

/**
 * @brief A filter implementation which provides the filter class for cost calculations based on segment probabilities.
 */
UCLASS()
class SHAREDSEARCHSPACE_API USearchSpaceLogicNavigationQueryFilter : public UNavFilter_AIControllerDefault 
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	USearchSpaceLogicNavigationQueryFilter();

	/**
	 * @brief Populate the cost calculation filter for segment probabilities.
	 * @param NavData The nav data to use.
	 * @param Querier The querier for the clique identified by the navdata.
	 * @param Filter The unconfigured filter to set the segment probabilities implementation.
	 */
	virtual void InitializeFilter(const ANavigationData& NavData, const UObject* Querier, FNavigationQueryFilter& Filter) const override;
	
};
