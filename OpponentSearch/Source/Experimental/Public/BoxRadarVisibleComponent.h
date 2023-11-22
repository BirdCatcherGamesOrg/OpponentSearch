// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BoxRadarVisibleComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EXPERIMENTALMODULE_API UBoxRadarVisibleComponent : public USceneComponent
{
	GENERATED_BODY()

public:

	UBoxRadarVisibleComponent();

	virtual void OnRegister() override;

	virtual void InitializeComponent() override;
	
	void SetDependencies(const class UAISenseConfig_Sight& PerceptionCone);
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class URadarVisibleComponent> RadarVisibleComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UStaticMeshComponent> CylinderComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UStaticMeshComponent> ConeComponent;
};
