// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "CliqueHandler.h"

UCliqueHandler::UCliqueHandler()
{
	PrimaryComponentTick.bCanEverTick = true;
}

const FCliqueHandle& UCliqueHandler::GetHandle() const
{
	return Handle;
}
