// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "CharacterRadar.h"

#include "BCIRadarDeveloperSettings.h"
#include "NativeGameplayTags.h"
#include "NativeGameplayTagUtilities.h"
#include "RadarCaptureComponent.h"
#include "RadarDeveloperSettings.h"
#include "RadarUIModel.h"
#include "RadarVisibleComponent.h"
#include "RadarVisibleComponentsSubsystem.h"
#include "Engine/TextureRenderTarget2D.h"

UCharacterRadarComponent::UCharacterRadarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetUsingAbsoluteRotation(true);
	RadarUIModel = CreateDefaultSubobject<URadarUIModel>("RadarUIModel");

	CylinderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderComponent"));
	CylinderComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	UStaticMesh* CylinderMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object;
	CylinderComponent->SetStaticMesh(CylinderMesh);

	RadarVisibleComponent = CreateDefaultSubobject<URadarVisibleComponent>(TEXT("RadarVisibleComponent"));

	const URadarDeveloperSettings* Settings = GetDefault<URadarDeveloperSettings>();
	RadarVisibleComponent->SetRadarVisibilityGroup(Settings->MovableStaticMeshRadarVisibilityTag);
}

void UCharacterRadarComponent::InitializeComponent()
{
	Super::InitializeComponent();
	RadarUIModel->SetRadarTexture(TextureTarget);
}

void UCharacterRadarComponent::BeginPlay()
{
	Super::BeginPlay();

	URadarVisibleComponentsSubsystem* RadarVisibleComponentsSubsystem =
		UWorld::GetSubsystem<URadarVisibleComponentsSubsystem>(GetWorld());

	
	const TMultiMap<FGameplayTag, TObjectPtr<UPrimitiveComponent>>& VisibleComponentsByGroup =
		RadarVisibleComponentsSubsystem->GetVisibleComponentsByGroup();

	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		URadarCaptureComponent* RadarCaptureComponent = Cast<URadarCaptureComponent>(Child);
		if (RadarCaptureComponent != nullptr)
		{
			TArray<TObjectPtr<UPrimitiveComponent>> VisibleComponents;
			VisibleComponentsByGroup.MultiFind(RadarCaptureComponent->GetRadarVisibilityGroup(), VisibleComponents);
			RadarCaptureComponent->SetVisibleComponents(VisibleComponents);
		}
	}
}

void UCharacterRadarComponent::OnRegister()
{
	Super::OnRegister();
	RadarUIModel->RegisterComponent();
	RadarVisibleComponent->RegisterComponent();
	RadarVisibleComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	CylinderComponent->RegisterComponent();
	CylinderComponent->AttachToComponent(RadarVisibleComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
}
