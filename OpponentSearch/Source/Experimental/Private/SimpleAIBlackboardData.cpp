// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "SimpleAIBlackboardData.h"
#include "CliqueHandler.h"
#include "SharedSearchSpaceBlackboardUtils.h"
#include "SharedSearchSpaceAgent.h"

void USimpleAIBlackboardData::PostInitProperties()
{
	if (HasAnyFlags(RF_NeedPostLoad | RF_ClassDefaultObject) == false)
	{
		AlarmStimulusLocationKey.MakeBlackboardEntry(AlarmStimulusLocationEntry);
		AlarmStimulusLocationEntry.EntryDescription = "The location of the alarm stimulus";
		Keys.AddUnique(AlarmStimulusLocationEntry);

		SharedSearchSpaceBlackboardUtils::CliqueHandlerKey.MakeBlackboardEntry(*this, CliqueHandlerEntry);
		SharedSearchSpaceBlackboardUtils::MoveToLocation.MakeBlackboardEntry(MoveToLocationEntry);
		SharedSearchSpaceBlackboardUtils::SharedSearchSpaceAgentKey.MakeBlackboardEntry(*this, PatrollerSightBoxEntry);
		SharedSearchSpaceBlackboardUtils::MoveToInitialAngleKey.MakeBlackboardEntry(MoveToInitialAngleEntry);
		Keys.AddUnique(CliqueHandlerEntry);
		Keys.AddUnique(MoveToLocationEntry);
		Keys.AddUnique(PatrollerSightBoxEntry);
		Keys.AddUnique(MoveToInitialAngleEntry);
	}
	
	Super::PostInitProperties();
}
