
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RadarWidget.generated.h"


UCLASS()
class BCIRADARUI_API URadarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<class UTextBlock> DetectionStatus;
	
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<class UImage> Radar;
	
};
