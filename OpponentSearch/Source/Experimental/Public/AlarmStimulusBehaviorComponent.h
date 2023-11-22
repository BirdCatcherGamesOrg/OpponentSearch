// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AlarmStimulusBehaviorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EXPERIMENTALMODULE_API UAlarmStimulusBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UAlarmStimulusBehaviorComponent();
	
	void SetDependencies(const class UCliqueHandler& InClique, class USenseConfigPublisher& SenseConfigPublisher, class AAIController& InAIController, class UPatrolSharedSearchSpaceComponent& InPatrolSharedSearchSpaceComponent, class UNativeSharedSearchSpaceLogicComponent& InSharedSearchSpaceLogicComponent);

	UFUNCTION(BlueprintCallable)
	const AActor* GetAlarmStimulusActor();
	
	void SetAlarmStimulusActor(class AActor& Actor);

	void EndAlarm();
	
	FVector GetNavAgentLocation() const;
	
private:

	UPROPERTY(Transient)
	TObjectPtr<const class AActor> ActorToPursue;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UNativeSharedSearchSpaceLogicComponent> NativeSharedSearchSpaceLogicComponent;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UPatrolSharedSearchSpaceComponent> PatrolSharedSearchSpaceComponent;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<class AAIController> AIController;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<const class UCliqueHandler> Clique;

	FDelegateHandle StimulusReactingDelayHandle;
	
	// Don't immediately lose stimulus, if say, the player rounds a corner.
	FTSTicker::FDelegateHandle DelayedLostStimulusHandle;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	float TimeToDelayLostStimulus = 5.0;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	float TimeToStayInSearch = 20.0;
	
};
