// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "ReferenceSharedSearchSpaceBlackboard.h"
#include "CliqueHandler.h"
#include "SharedSearchSpaceBlackboardUtils.h"
#include "SharedSearchSpaceAgent.h"

void UReferenceSharedSearchSpaceBlackboard::PostInitProperties()
{
	if (HasAnyFlags(RF_NeedPostLoad | RF_ClassDefaultObject) == false)
	{
		SharedSearchSpaceBlackboardUtils::CliqueHandlerKey.MakeBlackboardEntry(*this, CliqueHandlerEntry);
		SharedSearchSpaceBlackboardUtils::MoveToLocation.MakeBlackboardEntry(MoveToLocationEntry);
		SharedSearchSpaceBlackboardUtils::SharedSearchSpaceAgentKey.MakeBlackboardEntry(*this, PatrollerSightBoxEntry);
		Keys.AddUnique(CliqueHandlerEntry);
		Keys.AddUnique(MoveToLocationEntry);
		Keys.AddUnique(PatrollerSightBoxEntry);
	}
	
	Super::PostInitProperties();
}
