// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RadarVisibleComponent.generated.h"


/**
 * @brief A component which takes any child UPrimitiveComponents and registers them for a given radar visibility group.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BCIRADAR_API URadarVisibleComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	URadarVisibleComponent();

	/**
	 * @brief Registers child primitives with the radar subsystem.
	 */
	virtual void InitializeComponent() override;

	/**
	 * @brief Setter.
	 * @param RadarGroup The group to use.
	 */
	void SetRadarVisibilityGroup(const FGameplayTag& RadarGroup);
	
private:

	UPROPERTY(Config, EditAnywhere)
	FGameplayTag RadarVisibilityGroups;
	
};
