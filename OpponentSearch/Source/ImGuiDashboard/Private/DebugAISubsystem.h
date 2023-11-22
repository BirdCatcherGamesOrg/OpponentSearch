// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "DebugAISubsystem.generated.h"

USTRUCT()
struct FWaypointSegmentDebugInformation
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<class UProceduralMeshComponent> SegmentProceduralMesh;
	
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DynamicBasicShapeMaterialInstance;

	UPROPERTY()
	TObjectPtr<class UWidgetComponent> ProbabilityTextWidget;

	UPROPERTY()
	TObjectPtr<class UTextBlock> ProbabilityText;
	
};

/**
 * 
 */
UCLASS()
class IMGUIDASHBOARD_API UDebugAISubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	void ShowDashboard();

private:

	UPROPERTY()
	double PatrolTickRate = 0.0;
	
	UPROPERTY()
	bool bShowWaypoints = false;

	UPROPERTY()
	bool bShowPaths = false;

	UPROPERTY()
	bool bShowWaypointStaleness = false;
	
	TMap<TObjectPtr<class UNativeSharedSearchSpaceLogicComponent>, FDelegateHandle> PathCorridorUpdatedHandles;
	
	UPROPERTY()
	TArray<TObjectPtr<const class ARecastNavMesh>> RecastNavMeshes;

	UPROPERTY()
	int32 CurrentSelection = -1;

	UPROPERTY(Transient)
	TObjectPtr<class AActor> SegmentInformationComponentHolder;
	
	TMap<NavNodeRef, FWaypointSegmentDebugInformation> CurrentSegmentInformation;
	
	UPROPERTY(Transient)
	TObjectPtr<class UMaterialInstance> BasicShapeMaterial;

	UPROPERTY(Transient)
	TSubclassOf<class UFloatingTextWidget> FloatingTextWidgetType;

	UPROPERTY(Transient)
	TObjectPtr<class USharedSearchSpaceAgent> FakePatroller;
	
};


UCLASS()
class UFloatingTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<class UTextBlock> ProbabilityText;
	
};
