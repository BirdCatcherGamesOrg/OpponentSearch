// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CameraBoundsVolume.generated.h"

/**
 * @brief The allowed translation and rotations for a camera's movement within a bounded volume.
 */
USTRUCT()
struct FCameraBoundsAllowedTransforms
{
	GENERATED_BODY()

	/**
	 * @brief A fixed actor to track, if the player isn't being used.
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<class AActor> ActorToTrack;
	
	UPROPERTY(EditAnywhere)
	bool TranslateX = false;

	UPROPERTY(EditAnywhere)
	bool TranslateY = false;

	UPROPERTY(EditAnywhere)
	bool TranslateZ = false;

	/**
	 * @brief If rotating on the XAxis is allowed.
	 * Generally speaking, this should never be enabled. Rolling the camera is very nauseating.
	 */
	UPROPERTY(EditAnywhere)
	bool RotateX = false;
	
	UPROPERTY(EditAnywhere)
	bool RotateY = false;
	
	UPROPERTY(EditAnywhere)
	bool RotateZ = false;
};

/**
 * @brief An actor which automatically creates Box volumes and defines the transformations allowed.
 * Can be nested within each other, where players will have a stack of volumes.
 */
UCLASS()
class BCICINEMATICCAMERA_API ACameraBoundsVolume : public AActor
{
	GENERATED_BODY()

public:

	ACameraBoundsVolume();

	virtual void OnConstruction(const FTransform& Transform) override;
	
	const FCameraBoundsAllowedTransforms& GetCameraBoundsAllowedTransforms() const;

	/**
	 * @brief The area in which the player camera's movements will be decided by the volume's properties.
	 * This doesn't directly influence the player's camera location or rotation, but rather just
	 * the area where if the player is in, the camera will be moved.
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class ATriggerBox> ValidArea;

	/**
	 * @brief The area in which the camera can translate.
	 * The camera will move into this area and move and rotate around while the player is within the ValidArea box.
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class ATriggerBox> MovableArea;

	/**
	 * @brief The allowed transforms for the camera within the MovableArea.
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	FCameraBoundsAllowedTransforms CameraBoundsAllowedTransforms;

	/**
	 * @brief A fix positional offset for the camera.
	 * This allows the camera to view the player from other angles or positions than directly centered.
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	FTransform Offset;

	/**
	 * @brief The interpolation speed for camera translation.
	 * While not enforced, it is generally a good idea to only enable either translation or rotation interpolation,
	 * but not both at the same time. Otherwise the result is very disorienting.
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float InterpSpeed;

	/**
	 * @brief The interpolation speed for camera rotation.
	 * While not enforced, it is generally a good idea to only enable either translation or rotation interpolation,
	 * but not both at the same time. Otherwise the result is very disorienting.
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float RInterpSpeed;

	/**
	 * @brief The tolerance in angles for rotation interpolation before the camera snaps into position.
	 */
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float RInterpTolerance;
};
