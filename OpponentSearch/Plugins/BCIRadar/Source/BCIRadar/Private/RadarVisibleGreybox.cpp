// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.


#include "RadarVisibleGreybox.h"

#include "BCIRadarDeveloperSettings.h"
#include "RadarVisibleComponent.h"

ARadarVisibleGreybox::ARadarVisibleGreybox()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	StaticMeshComponent->SetStaticMesh(CubeMesh);
	UMaterial* BasicShapeMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'")).Object;
	StaticMeshComponent->SetMaterial(0, BasicShapeMaterial);
	SetRootComponent(StaticMeshComponent);
	RadarVisibleComponent = CreateDefaultSubobject<URadarVisibleComponent>(TEXT("RadarVisibleComponent"));
	RadarVisibleStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RadarVisibleStaticMeshComponent"));
	RadarVisibleStaticMeshComponent->SetStaticMesh(StaticMeshComponent->GetStaticMesh());
	RadarVisibleStaticMeshComponent->SetMaterial(0, StaticMeshComponent->GetMaterial(0));

	RadarVisibleComponent->SetupAttachment(StaticMeshComponent);
	RadarVisibleStaticMeshComponent->SetupAttachment(RadarVisibleComponent);

	const UBCIRadarDeveloperSettings* Settings = GetDefault<UBCIRadarDeveloperSettings>();
	RadarVisibleComponent->SetRadarVisibilityGroup(Settings->WorldStaticMeshRadarVisibilityTag);
}
