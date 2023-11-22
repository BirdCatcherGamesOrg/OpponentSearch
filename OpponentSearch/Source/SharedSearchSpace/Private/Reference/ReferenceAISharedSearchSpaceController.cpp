// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.


#include "ReferenceAISharedSearchSpaceController.h"

#include "CliqueHandler.h"
#include "SharedSearchSpaceAgent.h"
#include "ReferenceSharedSearchSpaceBlackboard.h"
#include "SharedSearchSpaceBlackboardUtils.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AReferenceAISharedSearchSpaceController::AReferenceAISharedSearchSpaceController()
{
	PrimaryActorTick.bCanEverTick = false;
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
	Blackboard->InitializeBlackboard(*GetMutableDefault<UReferenceSharedSearchSpaceBlackboard>());
}

void AReferenceAISharedSearchSpaceController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AReferenceAISharedSearchSpaceController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SharedSearchSpaceBlackboardUtils::CliqueHandlerKey.SetValue(*Blackboard, InPawn->FindComponentByClass<UCliqueHandler>());
	SharedSearchSpaceBlackboardUtils::SharedSearchSpaceAgentKey.SetValue(*Blackboard, InPawn->FindComponentByClass<USharedSearchSpaceAgent>());
	BrainComponent->StartLogic();
}
