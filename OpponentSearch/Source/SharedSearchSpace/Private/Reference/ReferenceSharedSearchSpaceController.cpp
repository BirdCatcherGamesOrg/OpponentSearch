// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "ReferenceSharedSearchSpaceController.h"
#include "CliqueHandler.h"
#include "PatrolSharedSearchSpaceComponent.h"
#include "NativeSharedSearchSpaceLogicComponent.h"
#include "SharedSearchSpaceAgent.h"

AReferenceSharedSearchSpaceController::AReferenceSharedSearchSpaceController()
{
	PrimaryActorTick.bCanEverTick = false;
	NativeSharedSearchSpaceLogicComponent = CreateDefaultSubobject<UNativeSharedSearchSpaceLogicComponent>(TEXT("NativeSharedSearchSpaceLogicComponent"));
	PatrolSharedSearchSpaceComponent = CreateDefaultSubobject<UPatrolSharedSearchSpaceComponent>(TEXT("PatrolSharedSearchSpaceComponent"));
}

void AReferenceSharedSearchSpaceController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const UCliqueHandler* CliqueHandler = InPawn->FindComponentByClass<UCliqueHandler>();
	USharedSearchSpaceAgent* PatrollerSightBox = InPawn->FindComponentByClass<USharedSearchSpaceAgent>();
	NativeSharedSearchSpaceLogicComponent->AttachToComponent(PatrollerSightBox, FAttachmentTransformRules::SnapToTargetIncludingScale);
	NativeSharedSearchSpaceLogicComponent->SetDependencies(
		*this,
		*CliqueHandler->GetHandle().CliqueIdentifier,
		*PatrollerSightBox,
		1);
	
	PatrolSharedSearchSpaceComponent->StartPatrolling(*this, *NativeSharedSearchSpaceLogicComponent);
}

void AReferenceSharedSearchSpaceController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetPawn()->GetActorLocation();
	OutRotation = GetPawn()->GetActorRotation();
}
