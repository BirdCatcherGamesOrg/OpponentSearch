// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CliqueHandle.h"
#include "Components/ActorComponent.h"
#include "CliqueHandler.generated.h"

/**
 * @brief A component which contains a clique handle.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BCICLIQUES_API UCliqueHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Constructor.
	 */
	UCliqueHandler();

	/**
	 * @brief Getter for the handle for the actor's clique.
	 * @return The handle.
	 */
	const FCliqueHandle& GetHandle() const;

private:

	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess=true))
	FCliqueHandle Handle;
	
};
