// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "DelegatingCameraActor.generated.h"

/**
 * @brief A camera component that delegates the view to an injected component.
 * Used by the ADelegatingCameraActor to setup the view target.
 */
UCLASS()
class BCICINEMATICCAMERA_API UDelegatingCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Inject the delegated camera component.
	 * @param NewDelegatedCameraComponent The delegated camera component.
	 */
	void SetDelegatedCameraComponent(class UCameraComponent& NewDelegatedCameraComponent);

	/**
	 * @copydoc UCameraComponent::GetCameraView
	 */
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

private:

	UPROPERTY()
	TObjectPtr<class UCameraComponent> DelegatedCameraComponent;

};

/**
 * @brief A child class of ACameraActor which delegates the camera view to an injected component.
 * Primarily used by APlayerCameraManager as a view target for the player.
 */
UCLASS()
class BCICINEMATICCAMERA_API ADelegatingCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 * Unlike most constructors, this uses legacy FObjectInitializer behavior to override the camera component
	 * of the ACameraActor base class.
	 * @param ObjectInitializer to use.
	 */
	ADelegatingCameraActor(const FObjectInitializer& ObjectInitializer);

	/**
	 * @brief Inject the delegated camera component.
	 * @param DelegatedCameraComponent The camera component to delegate to.
	 */
	void SetDelegatedCameraComponent(class UCameraComponent& DelegatedCameraComponent) const;
	
private:
	
	UPROPERTY()
	TObjectPtr<class UDelegatingCameraComponent> DelegatingCameraComponent;

};
