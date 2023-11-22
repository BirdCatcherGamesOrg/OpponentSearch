#include "ImGuiWidgetHelpers.h"

#include "imgui.h"

const FString ImGuiWidgetHelpers::DEFAULT_COMBO_PREVIEW_VALUE = TEXT("Select...");

bool ImGuiWidgetHelpers::ComboBuilder(const FString& Label, const TArray<FString>& Selectables, const int32 CurrentSelection, int32& OutNewSelection)
{
	bool bValueChanged = false;
	const FString* PreviewValue = &DEFAULT_COMBO_PREVIEW_VALUE;
	if (Selectables.IsValidIndex(CurrentSelection))
	{
		const FString& PreviewValueAsRef = Selectables[CurrentSelection];
		PreviewValue = &PreviewValueAsRef;
	}
	
	if (ImGui::BeginCombo(TCHAR_TO_ANSI(*Label), TCHAR_TO_ANSI(**PreviewValue)))
	{
		for (int32 i = 0; i < Selectables.Num(); i++)
		{
			bool bSelected = i == CurrentSelection;
			if (ImGui::Selectable(TCHAR_TO_ANSI(*Selectables[i])), bSelected)
			{
				OutNewSelection = i;
			}
			if (bSelected)
			{
				bValueChanged = true;
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
		return bValueChanged;
	}
	return bValueChanged;
}
