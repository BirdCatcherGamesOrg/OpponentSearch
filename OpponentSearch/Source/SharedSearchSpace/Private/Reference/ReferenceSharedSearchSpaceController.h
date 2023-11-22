// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ReferenceSharedSearchSpaceController.generated.h"

UCLASS()
class SHAREDSEARCHSPACE_API AReferenceSharedSearchSpaceController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReferenceSharedSearchSpaceController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UPatrolSharedSearchSpaceComponent> PatrolSharedSearchSpaceComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UNativeSharedSearchSpaceLogicComponent> NativeSharedSearchSpaceLogicComponent;
	
};
