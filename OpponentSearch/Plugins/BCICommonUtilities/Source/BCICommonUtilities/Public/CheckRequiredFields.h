// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#pragma once

#ifndef CHECK_REQUIRED_FIELDS
#include "BlueprintChecks.h"
#include "XLog.h"
#define CHECK_REQUIRED_FIELDS(LOGCATEGORY) \
	do { \
		if (!UBlueprintChecks::CheckRequiredFields(*this)) \
		{ \
			XLOG(LOGCATEGORY, Error, "Required field not set.", {}); \
			return;\
		} \
	} while (0)
#endif // CHECK_REQUIRED_FIELDS