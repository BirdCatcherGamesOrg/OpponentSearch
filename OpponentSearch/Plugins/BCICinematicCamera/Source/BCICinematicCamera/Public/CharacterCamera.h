// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterCamera.generated.h"

/**
 *
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BCICINEMATICCAMERA_API UCharacterCamera : public USceneComponent
{
	GENERATED_BODY()

public:

	UCharacterCamera();
	
	virtual void OnRegister() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;
	
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
