// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "DebugDashboard.h"

#include "AbilitySystemComponent.h"
#include "ConsoleVariableUtilities.h"
#include "DebugDashboardConstants.h"
#include "ImGuiDashboardLogCategory.h"
#include "ImGuiModule.h"
#include "UnrealSpdlog.h"
#include "Kismet/GameplayStatics.h"
#include "XLog.h"

TAutoConsoleVariable<bool> UDebugDashboard::CVarShowDebugDashboard(
	AUTO_CVAR_NAME("ShowDebugDashboard"),
	false,
	TEXT("Show the debug dashboard."));

void UDebugDashboard::Initialize(FSubsystemCollectionBase& Collection)
{
	LOG_INITIALIZE_SUBSYSTEM(LogImGuiDashboard);
	Super::Initialize(Collection);
	Constants = GetDefault<UDebugDashboardConstants>();
	CVarShowDebugDashboard->OnChangedDelegate().AddWeakLambda(this, [this](const IConsoleVariable* bShowDebugDashboard)
	{
		bShow = bShowDebugDashboard->GetBool();
	});
}

void UDebugDashboard::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	DebugAbilitySystemComponentHolder = InWorld.SpawnActor<AActor>();
	DebugAbilitySystemComponent = NewObject<UAbilitySystemComponent>(DebugAbilitySystemComponentHolder);
	DebugAbilitySystemComponentHolder->AddInstanceComponent(DebugAbilitySystemComponent);
	DebugAbilitySystemComponent->RegisterComponent();
}

void UDebugDashboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bShow)
	{
		FImGuiModule::Get().GetProperties().SetInputEnabled(false);
		return;
	}
	FImGuiModule::Get().GetProperties().SetInputEnabled(true);
	
	ImGui::Begin("Dissertation Sim Debug", nullptr, ImGuiWindowFlags_MenuBar);

	// Menus
	if (ImGui::BeginMenuBar())
	{
		for (const auto& Panel : ShowPanelCallbacks)
		{
			if (ImGui::Button(TCHAR_TO_ANSI(*Panel.Key)))
			{
				CurrentPanel = Panel.Key;
			}
		}

		ImGui::EndMenuBar();
	}

	FShowPanelSignature* OnShowPanel = ShowPanelCallbacks.Find(CurrentPanel);
	if (OnShowPanel != nullptr)
	{
		OnShowPanel->ExecuteIfBound();
	}
	/*
	// Content Panel
	if (bShowAI)
	{
		if (ImGui::Button(TCHAR_TO_ANSI(TEXT("Stun nearby Sentries"))))
		{
			for (TActorIterator<AAISecurityCamera> It(GetWorld()); It; ++It)
			{
				AAISecurityCamera* AISecurityCamera = *It;
				if (Character->GetDistanceTo(AISecurityCamera) > 10.0)
				{
					//continue;
				}
				UAbilitySystemComponent* AbilitySystemComponent = AISecurityCamera->GetAbilitySystemComponent();
				FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
				FGameplayEffectSpecHandle Spec =
					AbilitySystemComponent->MakeOutgoingSpec(Constants->StunGameplayEffect,  1, Context);
				AISecurityCamera->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			}
		}
	}

	if (bShowDetection)
	{
		if (ImGui::Button(TCHAR_TO_ANSI(TEXT("Trigger Detected at Location"))))
		{
			UPerceivableComponent* PerceivableComponent = Character->FindComponentByClass<UPerceivableComponent>();
			PerceivableComponent->PerceivedBy(*DebugAbilitySystemComponent);
		}
	}
	*/
	ImGui::End();
}

void UDebugDashboard::RegisterForShow(const FString& PanelName, FShowPanelSignature& OnShowPanel)
{
	ShowPanelCallbacks.Add(PanelName, OnShowPanel);
	//ShowPanelCallbacks.KeyStableSort(&FString::Compare);
}

TStatId UDebugDashboard::GetStatId() const
{
	return UObject::GetStatID();
}
