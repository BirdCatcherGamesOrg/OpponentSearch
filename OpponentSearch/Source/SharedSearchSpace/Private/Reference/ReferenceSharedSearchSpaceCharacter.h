// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ReferenceSharedSearchSpaceCharacter.generated.h"

UCLASS()
class SHAREDSEARCHSPACE_API AReferenceSharedSearchSpaceCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AReferenceSharedSearchSpaceCharacter();

private:

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UCliqueHandler> CliqueHandler;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class USharedSearchSpaceAgent> PatrollerSightBox;
	
};
