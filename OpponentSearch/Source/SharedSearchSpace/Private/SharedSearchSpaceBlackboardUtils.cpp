#include "SharedSearchSpaceBlackboardUtils.h"
#include "CliqueHandler.h"
#include "SharedSearchSpaceSubsystem.h"

const TNativeBlackboardKey<UBlackboardKeyType_Vector> SharedSearchSpaceBlackboardUtils::MoveToLocation = {TEXT("MoveToLocation"), 0};
const TNativeBlackboardKey<UBlackboardKeyType_Float> SharedSearchSpaceBlackboardUtils::MoveToInitialAngleKey = {TEXT("MoveToInitialAngle"), 0};
const TNativeObjectBlackboardKey<UCliqueHandler> SharedSearchSpaceBlackboardUtils::CliqueHandlerKey = {TEXT("CliqueHandler"), 0};
const TNativeObjectBlackboardKey<USharedSearchSpaceAgent> SharedSearchSpaceBlackboardUtils::SharedSearchSpaceAgentKey = {TEXT("SharedSearchSpaceAgent"), 0};

const ARecastNavMesh* SharedSearchSpaceBlackboardUtils::GetRecastNavMesh(USharedSearchSpaceSubsystem& SharedSearchSpaceSubsystem, FEnvQueryInstance& QueryInstance)
{
	UObject* QuerierObject = QueryInstance.Owner.Get();
	if (QuerierObject == nullptr)
	{
		return nullptr;
	}
	
	const AActor* QuerierActor = Cast<AActor>(QuerierObject);
	const UCliqueHandler* CliqueHandler = QuerierActor->FindComponentByClass<UCliqueHandler>();
	if (CliqueHandler == nullptr)
	{
		return nullptr;
	}

	const ARecastNavMesh* RecastNavMesh = CliqueHandler->GetHandle().CliqueIdentifier.Get();
	if (RecastNavMesh == nullptr)
	{
		return nullptr;
	}
	const bool bIsNavmeshTracked = SharedSearchSpaceSubsystem.IsTrackedNavMesh(*RecastNavMesh);
	if (!bIsNavmeshTracked)
	{
		return nullptr;
	}
	return RecastNavMesh;
}
