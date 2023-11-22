// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

/**
 * @brief Utility methods for skeletal meshes.
 */
struct CharacterSkeletalMeshUtils
{
	
	/**
	 * @brief Populate a skeletal mesh, useful for creating debug ACharacters or APawns.
	 * @param SkeletalMeshComponent to populate.
	 */
	BCICOMMONUTILITIES_API static void SetDefaultSkeletalMeshProperties(class USkeletalMeshComponent& SkeletalMeshComponent);
};
