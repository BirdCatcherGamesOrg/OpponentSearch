// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "EnvQueryGenerator_SharedSearchSpaceNodes.generated.h"

/**
 * @brief An implementation of an EQS generator which returns the nodes on navmesh for an actor with a clique handler.
 */
UCLASS()
class SHAREDSEARCHSPACE_API UEnvQueryGenerator_SharedSearchSpaceNodes : public UEnvQueryGenerator
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	UEnvQueryGenerator_SharedSearchSpaceNodes();

	/**
	 * @brief Generates a list of navigation points for the clique.
	 * @param QueryInstance Data for the specific invocation of the generator.
	 */
	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

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
