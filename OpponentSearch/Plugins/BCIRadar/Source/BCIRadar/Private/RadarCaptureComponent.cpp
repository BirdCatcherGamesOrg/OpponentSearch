// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "RadarCaptureComponent.h"
#include "Components/SceneCaptureComponent2D.h"

URadarCaptureComponent::URadarCaptureComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SceneCaptureComponent2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent2D"));
	SceneCaptureComponent2D->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCaptureComponent2D->ShowFlagSettings.Add({ FEngineShowFlags::FindNameByIndex(FEngineShowFlags::EShowFlag::SF_Atmosphere), false } );
}

void URadarCaptureComponent::OnRegister()
{
	Super::OnRegister();
	SceneCaptureComponent2D->RegisterComponent();
	SceneCaptureComponent2D->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void URadarCaptureComponent::SetVisibleComponents(const TArray<UPrimitiveComponent*>& VisibleComponents)
{
	SceneCaptureComponent2D->ShowOnlyComponents.Append(VisibleComponents);
}

const FGameplayTag& URadarCaptureComponent::GetRadarVisibilityGroup()
{
	return RadarVisibilityGroups;
}