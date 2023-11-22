#include "SpdlogOutputDevice.h"

#include "UnrealSpdlog.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"

class FDateTimeFlag : public spdlog::custom_flag_formatter
{
public:
	virtual void format(const spdlog::details::log_msg &, const std::tm &, spdlog::memory_buf_t &dest) override
	{
		const FDateTime DateTime = FDateTime::Now();
		const FString IsoDateTime = DateTime.ToIso8601();
		dest.append(GetData(IsoDateTime), GetData(IsoDateTime) + GetNum(IsoDateTime));
	}

	virtual std::unique_ptr<custom_flag_formatter> clone() const override
	{
		return spdlog::details::make_unique<FDateTimeFlag>();
	}
};

const FString FSpdlogOutputDevice::PATTERN = TEXT("{\"timestamp\":\"%J\",%v}");

struct FSpdlogLogger : TSharedFromThis<FSpdlogLogger>
{
	std::shared_ptr<spdlog::logger> logger;
};

FSpdlogOutputDevice::FSpdlogOutputDevice()
{
	Logger = MakeUnique<FSpdlogLogger>();
	const FString Filename = FPaths::Combine(FPaths::ProjectLogDir(), "spdlog.txt");
	Logger->logger = spdlog::daily_logger_st("daily_logger", TCHAR_TO_ANSI(*Filename), 2, 30);
	auto formatter = std::make_unique<spdlog::pattern_formatter>();
	formatter->add_flag<FDateTimeFlag>('J').set_pattern(TCHAR_TO_ANSI(*PATTERN));
	spdlog::set_formatter(std::move(formatter));
}

void FSpdlogOutputDevice::MakeSpdDefault() const
{
	spdlog::set_default_logger(Logger->logger);
}

void FSpdlogOutputDevice::Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category)
{
	UUnrealSpdlog::XLog(Category, Verbosity, "", 0, V, {});
}

void FSpdlogOutputDevice::Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category, const double Time)
{
	UUnrealSpdlog::XLog(Category, Verbosity, "", 0, V, {});
}

void FSpdlogOutputDevice::Flush()
{
	FOutputDevice::Flush();
	Logger->logger->flush();
}

bool FSpdlogOutputDevice::CanBeUsedOnAnyThread() const
{
	return true;
}

bool FSpdlogOutputDevice::CanBeUsedOnPanicThread() const
{
	return true;
}
