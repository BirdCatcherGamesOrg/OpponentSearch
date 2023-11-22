// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "SenseConfigPublisher.generated.h"

/**
 * @brief Callback on when perception is updated for a stimulus tag.
 */
DECLARE_EVENT_OneParam(USenseConfigPublisher, FSensedStimulusSignature, const FActorPerceptionUpdateInfo&);

/**
 * @brief 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BCIPERCEPTION_API USenseConfigPublisher : public UActorComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	USenseConfigPublisher();

	/**
	 * @brief Get all the senses set by Blueprints or Native C++.
	 * @return All active senses.
	 */
	const TArray<class UAISenseConfig*>& GetSenses();

	/**
	 * @brief Sets the UAIPerceptionComponent to subscribe to.
	 * @param InActivePerception The component to use.
	 * @return The number of senses successfully configured on the perception component.
	 */
	int32 SetActivePerception(class UAIPerceptionComponent& InActivePerception);

	/**
	 * @brief Add sense configs through C++.
	 * @param NativeSenses The configs to use.
	 */
	void AddNativeSenses(const TArray<class UAISenseConfig*>& NativeSenses);

	/**
	 * @brief Return the subscribable event for a stimulus tag.
	 * @param StimulusTag The stimulus tag to subscribe to.
	 * @return The event.
	 */
	FSensedStimulusSignature& OnSensed(const FName& StimulusTag);
	
private:
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<class UAISenseConfig>> AvailableSenses;
	
	UPROPERTY(Transient)
	TObjectPtr<class UAIPerceptionComponent> ActivePerception;
	
	TMap<FName, FSensedStimulusSignature> OnSensedByTag;
	
	UFUNCTION()
	void OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);
	
};
