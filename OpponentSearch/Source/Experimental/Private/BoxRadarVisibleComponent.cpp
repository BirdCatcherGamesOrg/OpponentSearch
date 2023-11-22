// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "BoxRadarVisibleComponent.h"

#include "BCIRadarDeveloperSettings.h"
#include "ExperimentalModuleSettings.h"
#include "RadarVisibleComponent.h"
#include "Perception/AISenseConfig_Sight.h"

UBoxRadarVisibleComponent::UBoxRadarVisibleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	RadarVisibleComponent = CreateDefaultSubobject<URadarVisibleComponent>(TEXT("RadarVisibleComponent"));

	// Cone is a bad approximation. Actually need to do a sector of a circle.
	ConeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConeComponent"));
	ConeComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	UStaticMesh* ConeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'")).Object;
	ConeComponent->SetStaticMesh(ConeMesh);
	
	CylinderComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderComponent"));
	CylinderComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	UStaticMesh* CylinderMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")).Object;
	CylinderComponent->SetStaticMesh(CylinderMesh);

	const UBCIRadarDeveloperSettings* Settings = GetDefault<UBCIRadarDeveloperSettings>();
	RadarVisibleComponent->SetRadarVisibilityGroup(Settings->MovableStaticMeshRadarVisibilityTag);
}

void UBoxRadarVisibleComponent::OnRegister()
{
	Super::OnRegister();
	RadarVisibleComponent->RegisterComponent();
	RadarVisibleComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	
	CylinderComponent->RegisterComponent();
	CylinderComponent->AttachToComponent(RadarVisibleComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	
	ConeComponent->RegisterComponent();
	ConeComponent->AttachToComponent(RadarVisibleComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	// Cone is a bad approximation. Actually need to do a sector of a circle.
	ConeComponent->SetRelativeRotation(FRotator::MakeFromEuler({120, 0, 90}));
}

void UBoxRadarVisibleComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UBoxRadarVisibleComponent::SetDependencies(const UAISenseConfig_Sight& PerceptionCone)
{
	float BaseScale = PerceptionCone.SightRadius / 100;
	const float XScale = FMath::Tan(FMath::DegreesToRadians(PerceptionCone.PeripheralVisionAngleDegrees));
	ConeComponent->SetRelativeLocation({PerceptionCone.SightRadius/2, 0, 0});
	
	ConeComponent->SetWorldScale3D({XScale * BaseScale, BaseScale, BaseScale});
}
