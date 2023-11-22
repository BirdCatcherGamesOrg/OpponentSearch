// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "UnrealSpdlogSubsystem.generated.h"

/**
 * @brief Create an output device that tee's all UE_LOG calls to spdlog.
 */
UCLASS()
class UNREALSPDLOG_API UUnrealSpdlogSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	
	/**
	 * @copydoc UEngineSubsystem::Initialize
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @copydoc UEngineSubsystem::Deinitialize
	 */
	virtual void Deinitialize() override;

private:

	TSharedPtr<class FSpdlogOutputDevice> OUTPUTDEVICE;

};
