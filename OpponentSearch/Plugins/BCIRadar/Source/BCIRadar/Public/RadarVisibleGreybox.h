// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadarVisibleGreybox.generated.h"

/**
 * @brief An AActor which is visible to a radar.
 * Useful for greybox level prototyping.
 */
UCLASS()
class BCIRADAR_API ARadarVisibleGreybox : public AActor
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	ARadarVisibleGreybox();

private:

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	TObjectPtr<class URadarVisibleComponent> RadarVisibleComponent;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	TObjectPtr<class UStaticMeshComponent> RadarVisibleStaticMeshComponent;

};
