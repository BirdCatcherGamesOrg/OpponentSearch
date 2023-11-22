#include "UnrealSpdlog.h"
#include "spdlog/spdlog.h"
#include "spdlog/pattern_formatter.h"
#pragma push_macro("check")
#undef check
#include "Kismet/KismetSystemLibrary.h"
#include "spdlog/fmt/bundled/ranges.h"
#include "spdlog/fmt/bundled/ostream.h"
#pragma pop_macro("check")

/*
template<>
struct fmt::formatter<ELogVerbosity::Type> : fmt::formatter<std::string>
{
	auto vformat(const ELogVerbosity::Type& my, format_context &ctx) -> decltype(ctx.out())
	{
		auto fmt = StringCast<ANSICHAR>(ToString(my));
		return vformat_to(ctx.out(), fmt.Get());
	}
};
*/

void UUnrealSpdlog::XLog(const UObject* WorldContextObject, const FString& InFunction, const FString& InString, bool bPrintToScreen, FLinearColor TextColor, float Duration, const FName Key)
{
	UKismetSystemLibrary::PrintString(WorldContextObject, InString, bPrintToScreen, false, TextColor, Duration, Key);
	XLog(LogBlueprintUserMessages, LogBlueprintUserMessages.GetVerbosity(), InFunction, 0, InString, {});
}

void UUnrealSpdlog::XLog(const FLogCategoryBase& LogCategory, const ELogVerbosity::Type Verbosity, const FString& File, const int32 Line, const FString& Message, const TMap<FString, FString>& Context)
{
	XLog(LogCategory.GetCategoryName(), Verbosity, File, Line, Message, Context);
}

void UUnrealSpdlog::XLog(const FName& LogCategory, const ELogVerbosity::Type Verbosity, const FString& File, const int32 Line, const FString& Message, const TMap<FString, FString>& Context)
{
	if (Message.EndsWith(REDIRECT_FILTER_SUFFIX))
	{
		return;
	}
	FString ContextString;
	FormatContext(Context, ContextString);
	spdlog::info("\"logCategory\":\"{}\",\"verbosity\":\"{}\",\"function\":{{\"name\":\"{}\",\"line\":{}}},\"message\":\"{}\",\"context\":[{}]", TCHAR_TO_ANSI(*LogCategory.ToString()), Verbosity, TCHAR_TO_ANSI(*File), Line, TCHAR_TO_ANSI(*Message.ReplaceQuotesWithEscapedQuotes()), TCHAR_TO_ANSI(*ContextString));
}

void UUnrealSpdlog::FormatContext(const TMap<FString, FString>& Context, FString& OutContext)
{
	FStringBuilderBase Result;
	bool First = true;
	for (const auto& Element : Context)
	{
		if (First)
		{
			First = false;
		}
		else
		{
			Result += ",";
		}

		Result += "{\"";
		Result += Element.Key.ReplaceQuotesWithEscapedQuotes();
		Result += "\":\"";
		Result += Element.Value.ReplaceQuotesWithEscapedQuotes();
		Result += "\"}";
	}
	OutContext = Result.ToString();
}
