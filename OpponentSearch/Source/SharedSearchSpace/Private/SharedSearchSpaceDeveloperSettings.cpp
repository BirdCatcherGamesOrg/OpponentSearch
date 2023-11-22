#include "SharedSearchSpaceDeveloperSettings.h"
#include "ConsoleVariableUtilities.h"
#include "HAL/IConsoleManager.h"

TAutoConsoleVariable<bool> USharedSearchSpaceDeveloperSettings::CVarEnablePatrolAI(
	AUTO_CVAR_NAME("EnablePatrolAI"),
	true,
	TEXT(""));

USharedSearchSpaceDeveloperSettings::USharedSearchSpaceDeveloperSettings()
{
	CVarEnablePatrolAI->OnChangedDelegate().AddWeakLambda(this, [this](const IConsoleVariable* EnablePatrolAI)
	{
		bEnablePatrolAI = EnablePatrolAI->GetBool();
		OnEnablePatrolAIEvent.Broadcast(bEnablePatrolAI);
	});
}

USharedSearchSpaceDeveloperSettings::FEnablePatrolAISignature& USharedSearchSpaceDeveloperSettings::OnEnablePatrolAI()
{
	return OnEnablePatrolAIEvent;
}
