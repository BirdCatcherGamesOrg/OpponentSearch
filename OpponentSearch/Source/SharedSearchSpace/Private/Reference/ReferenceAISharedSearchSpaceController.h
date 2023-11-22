// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ReferenceAISharedSearchSpaceController.generated.h"

UCLASS()
class SHAREDSEARCHSPACE_API AReferenceAISharedSearchSpaceController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReferenceAISharedSearchSpaceController();

	virtual void PostInitializeComponents() override;

	virtual void OnPossess(APawn* InPawn) override;
	
};
