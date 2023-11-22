// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RadarVisibleComponentsSubsystem.generated.h"

/**
 * @brief Stores all radar visible primitives to be retrieved later one.
 * This allows the player to toggle which groups to show using settings.
 */
UCLASS()
class BCIRADAR_API URadarVisibleComponentsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	/**
	 * @copydoc UWorldSubsystem::Initialize
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief Getter.
	 * @return The components by group.
	 */
	const TMultiMap<FGameplayTag, TObjectPtr<class UPrimitiveComponent>>& GetVisibleComponentsByGroup();

	/**
	 * @brief Adds a component to a given group.
	 * @param Group The group to use.
	 * @param PrimitiveComponent The component.
	 */
	void RegisterComponent(const FGameplayTag& Group, class UPrimitiveComponent& PrimitiveComponent);

private:

	TMultiMap<FGameplayTag, TObjectPtr<class UPrimitiveComponent>> VisibleComponentsByGroup;
	
};
