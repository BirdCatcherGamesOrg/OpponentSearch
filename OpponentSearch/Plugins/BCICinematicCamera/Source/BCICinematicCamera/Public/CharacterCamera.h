// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterCamera.generated.h"

/**
 * @brief Creates a cinematic camera component and integrates with ACameraBoundsVolumes.
 * This enables Actor specific cameras which focus on the Actor from customizable positions and angles
 * that are set by the level designer to provide cinematic POV's instead of an over the shoulder view. 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BCICINEMATICCAMERA_API UCharacterCamera : public USceneComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	UCharacterCamera();

	/**
	 * @copydoc USceneComponent::OnRegister
	 */
	virtual void OnRegister() override;

	/**
	 * @copydoc USceneComponent::TickComponent
	 * Camera position needs to be updated every tick.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @copydoc USceneComponent::InitializeComponent
	 */
	virtual void InitializeComponent() override;

	/**
	 * @brief A transient ACameraActor is spawned which is used as the view target for APlayerCameraManager::SetViewTarget.
	 * The CameraActor will see "through" the lens of the child UCineCameraComponent on this component.
	 * @see APlayerCameraManager::SetViewTarget
	 * @return The transient view target actor.
	 */
	class ACameraActor& GetViewTarget() const;

private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(Transient)
	TObjectPtr<class ACameraActor> ViewTarget;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCineCameraComponent> CineCameraComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UPrimitiveComponent> PrimitiveComponent;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<const class ACameraBoundsVolume>> CameraBoundsVolumeStack;
};
