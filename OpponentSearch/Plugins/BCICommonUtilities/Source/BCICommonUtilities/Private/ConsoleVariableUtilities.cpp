#include "ConsoleVariableUtilities.h"

const FString BCIConsoleVariables::Namespace = FString(PREPROCESSOR_TO_STRING(UE_PROJECT_NAME) ".").ToLower();

FString BCIConsoleVariables::MakeCVarName(FString CVarName)
{
	return Namespace + CVarName;
}

FString BCIConsoleVariables::MakeCVarName(FString ModuleName, FString CVarName)
{
	return Namespace + ModuleName + "." + CVarName;
}
