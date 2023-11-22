// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#ifndef XLOG

#include "UnrealSpdlog.h"
#define _STRINGIFY(x) #x
#define _TOSTRING(x) _STRINGIFY(x)

#define _XLOG_TO_UE(CATEGORY, VERBOSITY, MESSAGE, ...) \
	do { \
		FString ContextString; \
		UUnrealSpdlog::FormatContext({__VA_ARGS__}, ContextString); \
		UE_LOG(CATEGORY, VERBOSITY, TEXT(_TOSTRING(__FILE__) ":[" _TOSTRING(__LINE__) "]: " MESSAGE " - Context: %s - " REDIRECT_FILTER_SUFFIX), *ContextString); \
	} while (0)

#if USE_UNREAL_SPDLOG
/**
 * @brief Structured, contextual logging macro.
 * Wrapper over SpdLogging.
 * @param CATEGORY Logging category.
 * @param VERBOSITY Logging verbosity.
 * @param MESSAGE The message to log.
 * @param __VA_ARGS__ The TMap of logging context, passed as an initializer list of FString pairs.
 */
#define XLOG(CATEGORY, VERBOSITY, MESSAGE, ...) \
	do { \
		if (UE_LOG_ACTIVE(CATEGORY, VERBOSITY)) \
		{ \
			UUnrealSpdlog::XLog(CATEGORY, ELogVerbosity::VERBOSITY, __FILE__, __LINE__, MESSAGE, {__VA_ARGS__}); \
		} \
		_XLOG_TO_UE(CATEGORY, VERBOSITY, MESSAGE, __VA_ARGS__); \
	} while (0)
#else
/**
 * @brief Structure, contextual logging macro.
 * Wrapper over UE_LOG
 * @param CATEGORY Logging category.
 * @param VERBOSITY Logging verbosity.
 * @param MESSAGE The message to log.
 * @param __VA_ARGS__ The TMap of logging context, passed as an initializer list of FString pairs.
 */
#define XLOG(CATEGORY, VERBOSITY, MESSAGE, ...)  XLOG_TO_UE(CATEGORY, VERBOSITY, MESSAGE, __VA_ARGS__)
#endif // USE_UNREAL_SPDLOG
#endif // XLOG

#ifndef LOG_INITIALIZE_SUBSYSTEM
/**
 * Reusable logging macro for subsystems initialization.
 * @param CATEGORY Logging category.
 */
#define LOG_INITIALIZE_SUBSYSTEM(CATEGORY) XLOG(CATEGORY, Log, "Initializing subsystem", {"Subsystem", GetName()});
#endif // LOG_INITIALIZE_SUBSYSTEM

#ifndef LOG_DEINITIALIZE_SUBSYSTEM
/**
 * Reusable logging macro for subsystems deinitialization.
 * @param CATEGORY Logging category.
 */
#define LOG_DEINITIALIZE_SUBSYSTEM(CATEGORY) XLOG(CATEGORY, Log, "Deinitializing subsystem", {"Subsystem", GetName()});
#endif // LOG_DEINITIALIZE_SUBSYSTEM