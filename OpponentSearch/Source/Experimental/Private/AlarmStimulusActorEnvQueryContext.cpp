// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "AlarmStimulusActorEnvQueryContext.h"

#include "SimpleAIBlackboardData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

void UAlarmStimulusActorEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UObject* QuerierObject = QueryInstance.Owner.Get();
	if (QuerierObject == nullptr)
	{
		return;
	}
	
	AActor* QuerierActor = Cast<AActor>(QuerierObject);
	const UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(QuerierActor);
	if (BlackboardComponent == nullptr)
	{
		return;
	}
	
	const USimpleAIBlackboardData* SimpleAIBlackboardData = GetDefault<USimpleAIBlackboardData>();
	FVector AlarmStimulusLocation;
	const bool bHasAlarmStimulusLocation = SimpleAIBlackboardData->AlarmStimulusLocationKey.GetValue(*BlackboardComponent, AlarmStimulusLocation);
	if (!bHasAlarmStimulusLocation)
	{
		return;
	}
	
	UEnvQueryItemType_Point::SetContextHelper(ContextData, AlarmStimulusLocation);
}
