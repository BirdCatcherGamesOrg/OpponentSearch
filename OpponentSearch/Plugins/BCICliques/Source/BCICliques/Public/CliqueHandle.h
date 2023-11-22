// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NavMesh/RecastNavMesh.h"
#include "CliqueHandle.generated.h"

/**
 * @brief Handle for the AActor's clique.
 */
USTRUCT()
struct BCICLIQUES_API FCliqueHandle
{
	GENERATED_BODY()
	
	FCliqueHandle() = default;

	/**
	 * @brief Constructor.
	 * @param InCliqueIdentifier The recast navigation for the clique. 
	 */
	FCliqueHandle(const class ARecastNavMesh& InCliqueIdentifier);

	/**
	 * @brief Compare equality using the racast nav mesh.
	 * @param Other Comparable
	 * @return If the nav meshes are equal.
	 */
	bool operator==(const FCliqueHandle& Other) const;

	/**
	 * @brief UE implementation of equality.
	 * @param Other Comparable.
	 * @return If the nav meshes are equal.
	 */
	bool Equals(const FCliqueHandle& Other) const;

	/**
	 * @brief ToString implementation.
	 * @return The NavMesh's string.
	 */
	const FString& ToString() const;

	/**
	 * @brief Hash func.
	 * Uses the NavMesh's hash func.
	 * @return The hash.
	 */
	uint32 GetHashOfIdentifier() const;
	
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<const class ARecastNavMesh> CliqueIdentifier;

private:
	
	UPROPERTY()
	FString Name = "";
};

FORCEINLINE uint32 GetTypeHash(const FCliqueHandle& Handle)
{
	return Handle.GetHashOfIdentifier();
}
