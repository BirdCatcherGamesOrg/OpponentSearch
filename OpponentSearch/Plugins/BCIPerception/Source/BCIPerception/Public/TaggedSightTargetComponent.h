// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Perception/AISightTargetInterface.h"
#include "TaggedSightTargetComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BCIPERCEPTION_API UTaggedSightTargetComponent : public UActorComponent, public IAISightTargetInterface
{
	GENERATED_BODY()

public:

	UTaggedSightTargetComponent();

	void AddAISightTargetInterfaces(const TArray<class IAISightTargetInterface*>& InInterfaces);
	
	const FName& GetVisibleStimulusTag() const;

	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested, float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate) override;

private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess))
	FName VisibleStimulusTag = NAME_None;

	UPROPERTY()
	TArray<class IAISightTargetInterface*> Interfaces;
	
};
