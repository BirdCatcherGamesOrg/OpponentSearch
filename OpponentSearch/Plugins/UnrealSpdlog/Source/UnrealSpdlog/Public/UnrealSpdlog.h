// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnrealSpdlog.generated.h"

/**
 * @brief Suffix to help filter infinite loops from appearing in SPD log output.
 */
#define REDIRECT_FILTER_SUFFIX "[SPD-REDIRECT]"

/**
 * @brief Utils library for structured SPD logging.
 */
UCLASS()
class UNREALSPDLOG_API UUnrealSpdlog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	static void XLog(const UObject* WorldContextObject, const FString& InFunction, const FString& InString, bool bPrintToScreen, FLinearColor TextColor, float Duration, const FName Key);

	/**
	 * @brief Output logging through spd log.
	 * @param LogCategory To log under.
	 * @param Verbosity To use.
	 * @param File The file from the log statement.
	 * @param Line The line in the source file.
	 * @param Message The message to log.
	 * @param Context The structure context.
	 */
	static void XLog(const FLogCategoryBase& LogCategory, const ELogVerbosity::Type Verbosity, const FString& File, const int32 Line, const FString& Message, const TMap<FString, FString>& Context);

	/**
	 * @brief Output logging through spd log.
	 * @param LogCategory To log under.
	 * @param Verbosity To use.
	 * @param File The file from the log statement.
	 * @param Line The line in the source file.
	 * @param Message The message to log.
	 * @param Context The structure context.
	 */
	static void XLog(const FName& LogCategory, const ELogVerbosity::Type Verbosity, const FString& File, const int32 Line, const FString& Message, const TMap<FString, FString>& Context);

	/**
	 * @brief Join all context key/value pairs to a flattened string.
	 * @param Context Context values. 
	 * @param OutContext The flattened string.
	 */
	static void FormatContext(const TMap<FString, FString>& Context, FString& OutContext);

private:
	
	static TArray<FString> LoggedMessages;
	
};