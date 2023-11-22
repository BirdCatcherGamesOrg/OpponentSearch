// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ReferencePerceptionController.generated.h"

UCLASS()
class BCIPERCEPTION_API AReferencePerceptionController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReferencePerceptionController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	FName BlackboardKeyName;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	FName StimulusTag;
	
};
