// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

/**
 * @brief Create a CVar name with a consistent module namespace.
 * @param NAME Base name of the CVar.
 */
#define AUTO_CVAR_NAME(NAME) *(BCIConsoleVariables::MakeCVarName(TEXT(UE_MODULE_NAME), TEXT(NAME)))

/**
 * @brief Utils for CVars.
 * Expected names for CVars will be 'projectname.ModuleName.CVar'
 */
struct BCIConsoleVariables
{

	/**
	 * @brief Namespace for the CVars. Defaults to UE_PROJECT_NAME.
	 */
	BCICOMMONUTILITIES_API const static FString Namespace;

	/**
	 * @brief Create a CVar name with Namespace.
	 * @param CVarName The name to use.
	 */
	BCICOMMONUTILITIES_API static FString MakeCVarName(FString CVarName);

	/**
	 * @brief Create a CVar name with Namespace and a Module name.
	 * @param ModuleName The module in the CVar namespace.
	 * @param CVarName The name to use.
	 */
	BCICOMMONUTILITIES_API static FString MakeCVarName(FString ModuleName, FString CVarName);
	
};
