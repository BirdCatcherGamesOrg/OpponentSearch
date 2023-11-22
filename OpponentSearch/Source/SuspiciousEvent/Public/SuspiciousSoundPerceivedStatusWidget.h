// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SuspiciousSoundPerceivedStatusWidget.generated.h"

/**
 * @brief UMG Widget for when the opponent has perceived a suspicious sound.
 */
UCLASS()
class SUSPICIOUSEVENT_API USuspiciousSoundPerceivedStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Getter.
	 * @return The animation.
	 */
	class UWidgetAnimation* GetPerceivedSoundReactionAnimation() const;
	
private:

	UPROPERTY(EditDefaultsOnly, Transient, meta=(BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> PerceivedSoundReactionAnimation;
	
};
