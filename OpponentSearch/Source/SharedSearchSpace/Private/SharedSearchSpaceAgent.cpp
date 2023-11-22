// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "SharedSearchSpaceAgent.h"
#include "Components/BoxComponent.h"

USharedSearchSpaceAgent::USharedSearchSpaceAgent()
{
	PrimaryComponentTick.bCanEverTick = false;
	DebugPatrolBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DebugPatrolBoxComponent"));
	DebugPatrolBoxComponent->MarkAsEditorOnlySubobject();
	DebugPatrolBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void USharedSearchSpaceAgent::OnRegister()
{
	Super::OnRegister();
	if (DebugPatrolBoxComponent != nullptr)
	{
		DebugPatrolBoxComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}

const FVector& USharedSearchSpaceAgent::GetBoxExtent() const
{
	return BoxExtent;
}

#if WITH_EDITOR

void USharedSearchSpaceAgent::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	BoxExtent = DebugPatrolBoxComponent->GetScaledBoxExtent();
}

#endif // WITH_EDITOR
