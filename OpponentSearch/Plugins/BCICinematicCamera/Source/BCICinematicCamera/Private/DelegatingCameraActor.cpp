// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "DelegatingCameraActor.h"

void UDelegatingCameraComponent::SetDelegatedCameraComponent(UCameraComponent& NewDelegatedCameraComponent)
{
	DelegatedCameraComponent = &NewDelegatedCameraComponent;
}

void UDelegatingCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	if (DelegatedCameraComponent == nullptr)
	{
		return;
	}
	DelegatedCameraComponent->GetCameraView(DeltaTime, DesiredView);
}

ADelegatingCameraActor::ADelegatingCameraActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
			.SetDefaultSubobjectClass<UDelegatingCameraComponent>(TEXT("CameraComponent"))
	)
{
	PrimaryActorTick.bCanEverTick = false;
	DelegatingCameraComponent = Cast<UDelegatingCameraComponent>(GetCameraComponent());
}

void ADelegatingCameraActor::SetDelegatedCameraComponent(UCameraComponent& DelegatedCameraComponent) const
{
	DelegatingCameraComponent->SetDelegatedCameraComponent(DelegatedCameraComponent);
}
