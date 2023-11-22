// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "RadarVisibleComponent.h"
#include "RadarVisibleComponentsSubsystem.h"

URadarVisibleComponent::URadarVisibleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void URadarVisibleComponent::InitializeComponent()
{
	Super::InitializeComponent();
	URadarVisibleComponentsSubsystem* RadarVisibleComponentsSubsystem =
		UWorld::GetSubsystem<URadarVisibleComponentsSubsystem>(GetWorld());
	if (RadarVisibleComponentsSubsystem != nullptr)
	{
		TArray<USceneComponent*> Children;
		GetChildrenComponents(true, Children);
		for (USceneComponent* Child : Children)
		{
			UPrimitiveComponent* PrimitiveChild = Cast<UPrimitiveComponent>(Child);
			if (PrimitiveChild != nullptr)
			{
				PrimitiveChild->SetVisibleInSceneCaptureOnly(true);
				PrimitiveChild->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				RadarVisibleComponentsSubsystem->RegisterComponent(RadarVisibilityGroups, *PrimitiveChild);
			}
		}
	}
}

void URadarVisibleComponent::SetRadarVisibilityGroup(const FGameplayTag& RadarGroup)
{
	RadarVisibilityGroups = RadarGroup;
}
