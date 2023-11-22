#include "CharacterSkeletalMeshUtils.h"

#include "BCICommonUtilitiesLogCategory.h"
#include "XLog.h"

void CharacterSkeletalMeshUtils::SetDefaultSkeletalMeshProperties(USkeletalMeshComponent& SkeletalMeshComponent)
{
	auto& ThreadContext = FUObjectThreadContext::Get();
	if (!ThreadContext.IsInConstructor)
	{
		XLOG(LogBCICommonUtilities, Error, "Only set the default skeletal mesh properties from a constructor.",
			{{"threadID", FString::FromInt(FPlatformTLS::GetCurrentThreadId())},
			 {"component", SkeletalMeshComponent.GetName()},
			 {"actor", SkeletalMeshComponent.GetOwner()->GetName()}});
	}
	SkeletalMeshComponent.SetSkeletalMesh(ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'")).Object);
	SkeletalMeshComponent.SetAnimClass(ConstructorHelpers::FObjectFinder<UClass>(TEXT("AnimInstance'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C'")).Object);
	SkeletalMeshComponent.SetAnimationMode(EAnimationMode::AnimationBlueprint);
	SkeletalMeshComponent.SetRelativeLocation({0, 0, -90});
	SkeletalMeshComponent.SetRelativeRotation({0, 270, 0});
}
