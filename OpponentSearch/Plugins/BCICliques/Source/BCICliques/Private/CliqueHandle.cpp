// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "CliqueHandle.h"

FCliqueHandle::FCliqueHandle(const ARecastNavMesh& InCliqueIdentifier)
	:
CliqueIdentifier(&InCliqueIdentifier)
{
	Name = InCliqueIdentifier.GetName();
}

bool FCliqueHandle::operator==(const FCliqueHandle& Other) const
{
	return Equals(Other);
}
	
bool FCliqueHandle::Equals(const FCliqueHandle& Other) const
{
	return this->CliqueIdentifier == Other.CliqueIdentifier;
}

const FString& FCliqueHandle::ToString() const
{
	return Name;
}

uint32 FCliqueHandle::GetHashOfIdentifier() const
{
	if (CliqueIdentifier == nullptr)
	{
		return 0;
	}
	return GetTypeHash(CliqueIdentifier);
}