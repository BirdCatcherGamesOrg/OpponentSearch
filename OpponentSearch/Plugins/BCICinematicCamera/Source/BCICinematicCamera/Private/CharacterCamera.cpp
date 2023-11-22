// Copyright 2022 Bird Catcher Interactive. All Rights Reserved.

#include "CharacterCamera.h"

#include "CameraBoundsVolume.h"
#include "CineCameraComponent.h"
#include "DelegatingCameraActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

namespace
{
	void GetIdealCameraLocation(
		const ACameraBoundsVolume& Volume,
		const FCameraBoundsAllowedTransforms& AllowedTransforms,
		const FVector& ActorToTrackLocation,
		const FVector& CineCameraActorLocation,
		FVector& outIdealCameraLocation)
	{
		const FVector BoxPoint =
			Volume.MovableArea->GetComponentsBoundingBox(false, true).GetClosestPointTo(ActorToTrackLocation);
		const float IdealCameraLocationX = AllowedTransforms.TranslateX ? BoxPoint.X : CineCameraActorLocation.X;
		const float IdealCameraLocationY = AllowedTransforms.TranslateY ? BoxPoint.Y : CineCameraActorLocation.Y;
		const float IdealCameraLocationZ = AllowedTransforms.TranslateZ ? BoxPoint.Z : CineCameraActorLocation.Z;
		outIdealCameraLocation =
			FVector(IdealCameraLocationX, IdealCameraLocationY, IdealCameraLocationZ) + Volume.Offset.GetLocation();
	}

	void GetLookAtLocation(
		const ACameraBoundsVolume& Volume,
		const FCameraBoundsAllowedTransforms& AllowedTransforms,
		const FVector& ActorToTrackLocation,
		const FVector& IdealCameraLocation,
		FRotator& outRotator)
	{
		FVector LookAtLocation;
		LookAtLocation.X = AllowedTransforms.RotateY ? ActorToTrackLocation.X : IdealCameraLocation.X;
		LookAtLocation.Y = AllowedTransforms.RotateZ ? ActorToTrackLocation.Y : IdealCameraLocation.Y;
		LookAtLocation.Z = AllowedTransforms.RotateX ? ActorToTrackLocation.Z : IdealCameraLocation.Z;
	
		outRotator = UKismetMathLibrary::FindLookAtRotation(IdealCameraLocation, LookAtLocation) + Volume.Offset.GetRotation().Rotator();
	}

	void GetCameraTransformations(float DeltaTime, const ACameraBoundsVolume& CurrentCameraBoundsVolume, const FVector& CameraLocation, const FRotator& CameraRotation, const FVector& ActorToTrackLocation, FVector& outCameraLocation, FRotator& outRotator)
	{
		const FCameraBoundsAllowedTransforms& CameraBoundsAllowedTransforms =
			CurrentCameraBoundsVolume.GetCameraBoundsAllowedTransforms();
			
		FVector IdealCameraLocation;
		GetIdealCameraLocation(CurrentCameraBoundsVolume, CameraBoundsAllowedTransforms, ActorToTrackLocation, CameraLocation, IdealCameraLocation);
		FRotator LookAtRotation;
		GetLookAtLocation(CurrentCameraBoundsVolume, CameraBoundsAllowedTransforms, ActorToTrackLocation, CameraLocation, LookAtRotation);

		// Check against the interpolation threshold. Do not interpolate translation and rotation at the same time.
		// This results in a liquid-y motion-sickness inducing movement.
		const bool bIsInstantRotation = CurrentCameraBoundsVolume.RInterpSpeed == 0.0 ||
			CameraRotation.Equals(LookAtRotation, CurrentCameraBoundsVolume.RInterpTolerance);
		if (!bIsInstantRotation)
		{
			LookAtRotation = FMath::RInterpConstantTo(CameraRotation, LookAtRotation, DeltaTime, CurrentCameraBoundsVolume.RInterpSpeed);
		}

		if ((IdealCameraLocation - CameraLocation).Length() > 1.0)
		{
			IdealCameraLocation =
				FMath::VInterpTo(CameraLocation, IdealCameraLocation, DeltaTime, CurrentCameraBoundsVolume.InterpSpeed);
		}
	
		outRotator = LookAtRotation;
		outCameraLocation = IdealCameraLocation;
	}
}

UCharacterCamera::UCharacterCamera()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bWantsInitializeComponent = true;
	SetUsingAbsoluteRotation(true);
	CineCameraComponent = CreateDefaultSubobject<UCineCameraComponent>(TEXT("CineCameraComponent"));
	PrimitiveComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PrimitiveComponent"));
	PrimitiveComponent->OnComponentBeginOverlap.AddDynamic(this, &UCharacterCamera::OnBeginOverlap);
	PrimitiveComponent->OnComponentEndOverlap.AddDynamic(this, &UCharacterCamera::OnEndOverlap);
}

void UCharacterCamera::OnRegister()
{
	Super::OnRegister();
	CineCameraComponent->RegisterComponent();
	PrimitiveComponent->RegisterComponent();
	PrimitiveComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void UCharacterCamera::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector CameraLocation;
	FRotator CameraRotator;
	if (CameraBoundsVolumeStack.Num() == 0)
	{
		CameraLocation = GetComponentLocation();
		CameraRotator = GetComponentRotation();
		CineCameraComponent->SetWorldLocation(CameraLocation);
		CineCameraComponent->SetWorldRotation(CameraRotator);
		return;
	}
	
	const ACameraBoundsVolume* CurrentCameraBoundsVolume = CameraBoundsVolumeStack.Top();
	const AActor* ActorToTrack = GetOwner();
	if (CurrentCameraBoundsVolume->GetCameraBoundsAllowedTransforms().ActorToTrack != nullptr)
	{
		ActorToTrack = CurrentCameraBoundsVolume->GetCameraBoundsAllowedTransforms().ActorToTrack;
	}
	GetCameraTransformations(DeltaTime, *CameraBoundsVolumeStack.Top(), CineCameraComponent->GetComponentLocation(), CineCameraComponent->GetComponentRotation(), ActorToTrack->GetActorLocation(), CameraLocation, CameraRotator);
	CineCameraComponent->SetWorldLocation(CameraLocation);
	CineCameraComponent->SetWorldRotation(CameraRotator);
}

void UCharacterCamera::InitializeComponent()
{
	Super::InitializeComponent();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.ObjectFlags = RF_Transient;
	ADelegatingCameraActor* DelegatingCameraActor = GetWorld()->SpawnActor<ADelegatingCameraActor>(SpawnParameters);
	DelegatingCameraActor->SetDelegatedCameraComponent(*CineCameraComponent);
	ViewTarget = DelegatingCameraActor;
}

ACameraActor& UCharacterCamera::GetViewTarget() const
{
	return *ViewTarget;
}

void UCharacterCamera::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACameraBoundsVolume* CameraBoundsVolume = Cast<ACameraBoundsVolume>(OtherActor->GetAttachParentActor());
	if (CameraBoundsVolume == nullptr)
	{
		return;
	}
	CameraBoundsVolumeStack.Push(CameraBoundsVolume);
	SetComponentTickEnabled(true);
}

void UCharacterCamera::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const ACameraBoundsVolume* CameraBoundsVolume = Cast<ACameraBoundsVolume>(OtherActor->GetAttachParentActor());
	if (CameraBoundsVolume == nullptr)
	{
		return;
	}
	
	CameraBoundsVolumeStack.Pop();
	if (CameraBoundsVolumeStack.Num() == 0)
	{
		SetComponentTickEnabled(false);
	}
}
