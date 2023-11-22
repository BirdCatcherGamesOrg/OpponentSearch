// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopLevelWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTER_API UTopLevelWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Transient, meta=(BindWidget))
	TObjectPtr<class UNamedSlot> RadarSlot;
	
};
