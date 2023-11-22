#pragma once

/**
 * @brief Util methods for ImGui widgets.
 */
class ImGuiWidgetHelpers
{
public:
	
	/**
	 * @brief The string to show in the combo box before any values are selected.
	 */
	static const FString DEFAULT_COMBO_PREVIEW_VALUE;

	/**
	 * @brief Construct a combobox from an array of objects that can be represented as FStrings.
	 * @tparam Selectable The type of the selectable object.
	 * @tparam Transform The transformation to produce an FString from the selectable.
	 * @param Label ComboBox label.
	 * @param Selectables All selectables.
	 * @param SelectableTransform The transformation function ptr.
	 * @param CurrentSelection The index of the current selection.
	 * @param OutNewSelection The new selection when the function returns true.
	 * @return Whether the user has chosen a new combo entry.
	 */
	template
	<typename Selectable, typename Transform>
	static bool ComboBuilder(const FString& Label, const TArray<Selectable> Selectables, Transform SelectableTransform, const int32 CurrentSelection, int32& OutNewSelection);

	/**
	 * @brief Construct a combobox from an array of FStrings.
	 * Not a specialization of the above, as no transform parameter is needed.
	* @param Label ComboBox label.
	 * @param Selectables All selectables.
	 * @param CurrentSelection The index of the current selection.
	 * @param OutNewSelection The new selection when the function returns true.
	 * @return Whether the user has chosen a new combo entry.
	 */
	static bool ComboBuilder(const FString& Label, const TArray<FString>& Selectables, const int32 CurrentSelection, int32& OutNewSelection);
	
private:

	static TArray<FString> TemporaryComboStringHolder;
	
};

template <typename Selectable, typename Transform>
inline bool ImGuiWidgetHelpers::ComboBuilder(const FString& Label, const TArray<Selectable> Selectables, Transform SelectableTransform, const int32 CurrentSelection, int32& OutNewSelection)
{
	TemporaryComboStringHolder.Reset(Selectables.Num());
	Algo::Transform(Selectables, TemporaryComboStringHolder, SelectableTransform);
	return ComboBuilder(Label, TemporaryComboStringHolder, CurrentSelection, OutNewSelection);
}

