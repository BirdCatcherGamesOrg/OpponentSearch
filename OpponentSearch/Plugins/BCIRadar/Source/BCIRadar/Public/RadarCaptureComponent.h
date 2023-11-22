// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "RadarCaptureComponent.generated.h"

/**
 * @brief Component which draws primitives onto the radar texture using the configured scene capture component.
 * This forms the basis of a classic 2d overhead minimap.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BCIRADAR_API URadarCaptureComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	URadarCaptureComponent();

	/**
	 * @brief Register child components.
	 */
	virtual void OnRegister() override;

	/**
	 * @brief Set which components are opted into being drawn onto the radar.
	 * The capture component sets ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList, so this is that allow list.
	 * @param VisibleComponents The components to draw.
	 */
	void SetVisibleComponents(const TArray<class UPrimitiveComponent*>& VisibleComponents);

	// TODO: Remember what this is used for.
	/**
	 * @brief I don't remember.
	 * @return 
	 */
	const FGameplayTag& GetRadarVisibilityGroup();

private:
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class USceneCaptureComponent2D> SceneCaptureComponent2D;

	UPROPERTY(Config, EditAnywhere, meta = (Categories = "Radar.Visibility"))
	FGameplayTag RadarVisibilityGroups;
	
};
