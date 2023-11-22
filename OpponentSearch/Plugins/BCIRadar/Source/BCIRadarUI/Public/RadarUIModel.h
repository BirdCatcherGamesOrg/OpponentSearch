
#pragma once

#include "CoreMinimal.h"
#include "RadarUISettings.h"
#include "RadarUIModel.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BCIRADARUI_API URadarUIModel : public UActorComponent
{
	GENERATED_BODY()

public:

	void SetRadarTexture(class UTexture* RadarTexture);

	void SetPerceivedState(const FPerceivedState& PerceivedState);
	
	class URadarWidget* SetWidgetTree(class UWidgetTree* WidgetTree);
	
private:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterialInterface> RadarMaterial;

	UPROPERTY(EditAnywhere, meta=(GetOptions="GetRadarMaterialParameterNameOptions"))
	FName RadarMaterialParameterName;
	
	UPROPERTY(Transient)
	TObjectPtr<class UMaterialInstanceDynamic> RadarMaterialInstance;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TSubclassOf<class URadarWidget> RadarWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<class URadarWidget> RadarWidget;

	UFUNCTION()
	TArray<FString> GetRadarMaterialParameterNameOptions() const;
	
};