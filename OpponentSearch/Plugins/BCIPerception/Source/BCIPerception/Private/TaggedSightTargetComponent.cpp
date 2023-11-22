// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "TaggedSightTargetComponent.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UTaggedSightTargetComponent::UTaggedSightTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTaggedSightTargetComponent::AddAISightTargetInterfaces(const TArray<IAISightTargetInterface*>& InInterfaces)
{
	Interfaces.Append(InInterfaces);
}

const FName& UTaggedSightTargetComponent::GetVisibleStimulusTag() const
{
	return VisibleStimulusTag;
}

UAISense_Sight::EVisibilityResult UTaggedSightTargetComponent::CanBeSeenFrom(const FCanBeSeenFromContext& Context,
	FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested,
	float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	float TotalSightStrength = 0.0;
	for (IAISightTargetInterface* Interface : Interfaces)
	{
		float InterfaceSightStrength = 0.0;
		const UAISense_Sight::EVisibilityResult InterfaceResult = Interface->CanBeSeenFrom(Context, OutSeenLocation, OutNumberOfLoSChecksPerformed, OutNumberOfAsyncLosCheckRequested, InterfaceSightStrength, UserData, Delegate);
		if (InterfaceResult == UAISense_Sight::EVisibilityResult::Pending)
		{
			// TODO: Support Pending.
			continue;
		}
		
		if (FMath::IsNearlyEqual(InterfaceSightStrength, TNumericLimits<float>::Lowest()))
		{
			OutSightStrength = 0.0;
			return UAISense_Sight::EVisibilityResult::NotVisible;
		}
		if (FMath::IsNearlyEqual(InterfaceSightStrength, TNumericLimits<float>::Max()))
		{
			OutSightStrength = 1.0;
			return UAISense_Sight::EVisibilityResult::Visible;
		}
		
		TotalSightStrength += InterfaceSightStrength;
	}
	
	OutSightStrength = UKismetMathLibrary::SafeDivide(TotalSightStrength, Interfaces.Num());
	return OutSightStrength >= 0.5 ? UAISense_Sight::EVisibilityResult::Visible : UAISense_Sight::EVisibilityResult::NotVisible;
}
