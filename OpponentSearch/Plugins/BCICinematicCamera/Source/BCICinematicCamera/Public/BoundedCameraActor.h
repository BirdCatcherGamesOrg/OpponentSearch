// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoundedCameraActor.generated.h"

UCLASS()
class BCICINEMATICCAMERA_API ABoundedCameraActor : public AActor
{
	GENERATED_BODY()

public:

	ABoundedCameraActor();

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class ACameraBoundsVolume> TopLevelCameraBoundsVolume;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

};
