// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugDashboard.generated.h"

DECLARE_DELEGATE(FShowPanelSignature);

/**
 * 
 */
UCLASS()
class IMGUIDASHBOARD_API UDebugDashboard : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	virtual void Tick(float DeltaTime) override;

	void RegisterForShow(const FString& PanelName, FShowPanelSignature& OnShowPanel);
	
	virtual TStatId GetStatId() const override;

private:

	static TAutoConsoleVariable<bool> CVarShowDebugDashboard;
	
	UPROPERTY(Transient)
	bool bShow = false;
	
	UPROPERTY(Transient)
	TObjectPtr<const class UDebugDashboardConstants> Constants;
	
	UPROPERTY(Transient)
	TObjectPtr<class AActor> DebugAbilitySystemComponentHolder;

	UPROPERTY(Transient)
	TObjectPtr<class UAbilitySystemComponent> DebugAbilitySystemComponent;

	UPROPERTY(Transient)
	FString CurrentPanel = TEXT("");
	
	TMap<FString, FShowPanelSignature> ShowPanelCallbacks;
};
