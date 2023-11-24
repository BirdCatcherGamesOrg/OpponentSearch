// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoundedCameraActor.generated.h"

/**
 * @brief An actor which simply places a camera bounds volume in the world.
 * This creates the top of a nestable set of camera bounds.
 */
UCLASS()
class BCICINEMATICCAMERA_API ABoundedCameraActor : public AActor
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	
	ABoundedCameraActor();

	/**
	 * @brief The top of the nest of volumes.
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class ACameraBoundsVolume> TopLevelCameraBoundsVolume;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

};
