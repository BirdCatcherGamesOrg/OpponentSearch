// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CameraBoundsVolume.generated.h"

USTRUCT()
struct FCameraBoundsAllowedTransforms
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AActor> ActorToTrack;
	
	UPROPERTY(EditAnywhere)
	bool TranslateX = false;

	UPROPERTY(EditAnywhere)
	bool TranslateY = false;

	UPROPERTY(EditAnywhere)
	bool TranslateZ = false;

	UPROPERTY(EditAnywhere)
	bool RotateX = false;

	UPROPERTY(EditAnywhere)
	bool RotateY = false;

	UPROPERTY(EditAnywhere)
	bool RotateZ = false;
};

UCLASS()
class BCICINEMATICCAMERA_API ACameraBoundsVolume : public AActor
{
	GENERATED_BODY()

public:

	ACameraBoundsVolume();

	virtual void OnConstruction(const FTransform& Transform) override;
	
	const FCameraBoundsAllowedTransforms& GetCameraBoundsAllowedTransforms() const;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class ATriggerBox> ValidArea;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class ATriggerBox> MovableArea;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	FCameraBoundsAllowedTransforms CameraBoundsAllowedTransforms;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	FTransform Offset;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float InterpSpeed;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float RInterpSpeed;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float RInterpTolerance;
};
