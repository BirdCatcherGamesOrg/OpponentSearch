// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "ReferencePerceptionController.h"
#include "PerceptionLogCategory.h"
#include "SenseConfigPublisher.h"
#include "XLog.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

AReferencePerceptionController::AReferencePerceptionController()
{
	PrimaryActorTick.bCanEverTick = false;
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AReferencePerceptionController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	USenseConfigPublisher* SenseConfigPublisher = InPawn->FindComponentByClass<USenseConfigPublisher>();
	if (SenseConfigPublisher == nullptr)
	{
		XLOG(LogPerception, Warning, "Requires Senses to function.");
		return;
	}
	
	SenseConfigPublisher->SetActivePerception(*GetPerceptionComponent());
	SenseConfigPublisher->OnSensed(StimulusTag).AddWeakLambda(this, [this](const FActorPerceptionUpdateInfo& Info)
	{
		GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKeyName, Info.Target.Get());
	});
}

void AReferencePerceptionController::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = GetPawn()->GetActorLocation();
	OutRotation = GetPawn()->GetActorRotation();
}

