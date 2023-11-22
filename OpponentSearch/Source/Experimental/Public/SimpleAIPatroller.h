// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SimpleAIPatroller.generated.h"

UCLASS()
class EXPERIMENTALMODULE_API ASimpleAIPatroller : public ACharacter
{
	GENERATED_BODY()

public:

	ASimpleAIPatroller();

	virtual void PostInitializeComponents() override;

private:

	// Patrolling Components
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCliqueHandler> CliqueHandler;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class USenseConfigPublisher> SenseConfigPublisher;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class USharedSearchSpaceAgent> SharedSearchSpaceAgent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UBoxRadarVisibleComponent> BoxRadarVisibleComponent;
	
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Hearing> NativeHearing;

	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> NativeVision;
	
};
