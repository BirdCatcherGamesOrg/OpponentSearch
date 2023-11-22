// Copyright 2023 Bird Catcher Interactive. All Rights Reserved.

#include "SuspiciousSoundEmitter.h"
#include "AbilitySystemComponent.h"
#include "EmitSuspiciousSound.h"
#include "NativeGameplayTags.h"
#include "NativeGameplayTagUtilities.h"
#include "Components/AudioComponent.h"

namespace
{
	DEFINE_TAG_STATIC(ASuspiciousSoundEmitter, EmitSoundActivationTag);
}

ASuspiciousSoundEmitter::ASuspiciousSoundEmitter()
{
	PrimaryActorTick.bCanEverTick = false;
	Payload = CreateDefaultSubobject<UEmitSuspiciousSoundPayload>(TEXT("Payload"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	SoundEmitter = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));

	
	UStaticMeshComponent* Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	Cube->SetStaticMesh(CubeMesh);
	SetRootComponent(Cube);
	Cube->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SoundEmitter->SetupAttachment(RootComponent);
}

void ASuspiciousSoundEmitter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Payload->AudioComponent = SoundEmitter;
	Payload->AudioParameters.Append(AudioParameters);
	GameplayEventData.OptionalObject = Payload;
	FGameplayAbilitySpec Spec(EmitSoundAbilityType);
	Spec.DynamicAbilityTags.AddTag(EmitSoundActivationTag);
	EmitSuspiciousSoundHandle = AbilitySystemComponent->GiveAbility(Spec);
}

void ASuspiciousSoundEmitter::Emit(UAIPerceptionSystem& AIListeners, float Loudness)
{
	Payload->AIListeners = &AIListeners;
	Payload->Loudness = Loudness;
	FGameplayAbilityActorInfo ActorInfo;
	ActorInfo.InitFromActor(this, this, AbilitySystemComponent);
	AbilitySystemComponent->TriggerAbilityFromGameplayEvent(EmitSuspiciousSoundHandle, &ActorInfo, EmitSoundActivationTag, &GameplayEventData, *AbilitySystemComponent);
}
