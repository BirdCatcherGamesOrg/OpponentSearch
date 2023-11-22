#include "RadarUIModel.h"

#include "RadarUISettings.h"
#include "RadarWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URadarUIModel::SetRadarTexture(UTexture* RadarTexture)
{
	RadarMaterialInstance =
		UMaterialInstanceDynamic::Create(RadarMaterial.Get(), this, "RadarMaterialInstance");
	RadarMaterialInstance->SetTextureParameterValue(RadarMaterialParameterName, RadarTexture);
}

void URadarUIModel::SetPerceivedState(const FPerceivedState& PerceivedState)
{
	RadarWidget->DetectionStatus->SetText(PerceivedState.Text);
}

URadarWidget* URadarUIModel::SetWidgetTree(UWidgetTree* WidgetTree)
{
	RadarWidget = WidgetTree->ConstructWidget<URadarWidget>(*RadarWidgetClass, "RadarWidget");
	RadarWidget->Radar->SetBrushFromMaterial(RadarMaterialInstance);
	return RadarWidget;
}

TArray<FString> URadarUIModel::GetRadarMaterialParameterNameOptions() const
{
	TArray<FString> Options;
	if (RadarMaterial == nullptr)
	{
		return Options;
	}
	TArray<FMaterialParameterInfo> ParameterInfo;
	TArray<FGuid> ParameterIds;
	RadarMaterial->GetAllTextureParameterInfo(ParameterInfo, ParameterIds);
	Algo::Transform(ParameterInfo, Options, [](const FMaterialParameterInfo& Info)
	{
		return Info.Name.ToString();
	});
	return Options;
}
