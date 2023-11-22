// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SharedSearchSpaceAgent.generated.h"

/**
 * @brief A component which manages the sight box for observing waypoints in the USharedSearchSpaceSubsystem.
 * This component also acts as the key for the owning Actor to integrate with the system.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHAREDSEARCHSPACE_API USharedSearchSpaceAgent : public USceneComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	USharedSearchSpaceAgent();

	/**
	 * @brief Register the child components.
	 */
	virtual void OnRegister() override;

	/**
	 * @brief Get the extent of the sight box.
	 * Search space waypoints are compared against the box extent. If the waypoint falls within the box,
	 * then it is a candidate for being marked as seen, pending a line trace.
	 * 
	 * For visualization purposes, this component has an editor only box component.
	 * This helps designers tune the size of the waypoint discovery box.
	 * The extent of the editor box is saved, but the box itself is unused during runtime
	 * since hit or collision detection is unneeded and is suboptimal compared to an extent check.
	 * @return the extent.
	 */
	const FVector& GetBoxExtent() const;

#if WITH_EDITOR

	/**
	 * In editor, copy the extent from the debug box to the saved properties.
	 * @param PropertyChangedEvent The event of the editor change.
	 */
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	
#endif // WITH_EDITOR
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UBoxComponent> DebugPatrolBoxComponent;

	UPROPERTY(VisibleAnywhere)
	FVector BoxExtent;
	
};
