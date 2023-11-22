// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterRadar.generated.h"

/**
 * @brief Player character with the radar system as a child component.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CHARACTER_API UCharacterRadarComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	UCharacterRadarComponent();

	/**
	 * @copydoc USceneComponent::InitializeComponent.
	 */
	virtual void InitializeComponent() override;

	/**
	 * @copydoc USceneComponent::BeginPlay
	 */
	virtual void BeginPlay() override;

	/**
	 * @copydoc USceneComponent::OnRegister
	 */
	virtual void OnRegister() override;

private:
	
	/** Output render target of the scene capture that can be read in materials. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SceneCapture, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UTextureRenderTarget2D> TextureTarget;
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class URadarUIModel> RadarUIModel;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class URadarVisibleComponent> RadarVisibleComponent;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UStaticMeshComponent> CylinderComponent;

};
