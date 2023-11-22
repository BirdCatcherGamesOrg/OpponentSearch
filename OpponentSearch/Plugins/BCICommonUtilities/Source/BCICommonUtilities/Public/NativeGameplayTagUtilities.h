// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreGlobals.h"
#include "NativeGameplayTags.h"

/**
 * @brief Wrapper for static gameplay tag definitions with a consistent project, module and class namespace.
 * @param UCLASS The associated "owner" of the tag.
 * @param TAG The tag name.
 */
#define DEFINE_TAG_STATIC(UCLASS, TAG) UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG, PREPROCESSOR_TO_STRING(UE_PROJECT_NAME) "." UE_MODULE_NAME "." # UCLASS "." # TAG)
