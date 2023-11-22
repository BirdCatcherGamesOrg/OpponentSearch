// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_SharedSearchSpace.generated.h"

/**
 * @brief An EQS test which retrieves the segment probabilities from USharedSearchSpaceSubsystem.
 * Designed only to be used with UEnvQueryGenerator_SharedSearchSpaceNodes.
 */
UCLASS()
class SHAREDSEARCHSPACE_API UEnvQueryTest_SharedSearchSpace : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	UEnvQueryTest_SharedSearchSpace();

	/**
	 * @brief Retrieve the segment probabilities from the USharedSearchSpaceSubsystem.
	 * @param QueryInstance 
	 */
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	/**
	 * @brief Get the title for the EQS graph node in editor.
	 * @return The title.
	 */
	virtual FText GetDescriptionTitle() const override;

	/**
	 * @brief Get the description for the EQS graph node in editor.
	 * @return The description.
	 */
	virtual FText GetDescriptionDetails() const override;

};
