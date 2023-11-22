#pragma once

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

/**
 * @brief Provides compile time type safety for blackboard entries with a specific name.
 * Meant to be used in C++ code in base blackboard assets.
 * This avoids needing to look up blackboard keys with TEXT macros or a const string.
 * @tparam T the type of the BlackboardKeyEntry to use.
 */
template
<typename T>
struct TNativeBlackboardKey
{
	/**
	 * @brief Used to get the type with declval.
	 * Example usage: FBBKeyCachedAccessor<decltype(USimpleAIBlackboardData::AlarmStimulusLocationKey)::Type>
	 */
	using Type = T;

	/**
	 * @brief Constructor.
	 * @param InName Name of the key to use.
	 * @param InbInstanceSynced If the value is synced across all instances.
	 */
	TNativeBlackboardKey(const FName& InName, uint32 InbInstanceSynced = 1) : Name(InName), bInstanceSynced(InbInstanceSynced)
	{
		static_assert(TIsDerivedFrom<T, UBlackboardKeyType>::IsDerived, "Only UBlackboardKeyType types.");
		static_assert(!std::is_same_v<T, UBlackboardKeyType>, "Base UBlackboardKeyType is not supported. Use a derived class.");
		static_assert(!TIsDerivedFrom<T, UBlackboardKeyType_Object>::IsDerived, "UBlackboardKeyType_Object is not supported. Use TNativeObjectBlackboardKey.");
	};

	/**
	 * @brief Populate the values of a FBlackboardEntry.
	 * @param OutBlackboardEntry 
	 */
	void MakeBlackboardEntry(FBlackboardEntry& OutBlackboardEntry) const
	{
		OutBlackboardEntry.KeyType = const_cast<T*>(GetDefault<T>());
		OutBlackboardEntry.EntryName = Name;
	}

	/**
	 * @brief Populate the values of a FBBKeyCachedAccessor for a UBlackboardComponent instance.
	 * @param BlackboardComponent Instance to access.
	 * @param OutBBKeyCachedAccessor The accessor.
	 */
	void MakeBBKeyCachedAccessor(UBlackboardComponent& BlackboardComponent, FBBKeyCachedAccessor<T>& OutBBKeyCachedAccessor) const
	{
		FBlackboard::FKey KeyID = BlackboardComponent.GetBlackboardAsset()->GetKeyID(Name);
		OutBBKeyCachedAccessor = FBBKeyCachedAccessor<T>(BlackboardComponent, KeyID);
	}

	/**
	 * @brief Retrieve the value directly for a UBlackboardComponent instance.
	 * @param BlackboardComponent Instance to access.
	 * @param OutDataType Value to populate.
	 * @return Whether the instance has the key.
	 */
	bool GetValue(const UBlackboardComponent& BlackboardComponent, typename T::FDataType& OutDataType) const
	{
		const FBlackboard::FKey KeyID = BlackboardComponent.GetBlackboardAsset()->GetKeyID(Name);
		if (KeyID == FBlackboard::InvalidKey)
		{
			return false;
		}
		OutDataType = BlackboardComponent.GetValue<T>(Name);
		return true;
	}

	/**
	 * @brief Retrieve the value indirectly for a UBlackboardComponent instance using a FBlackboardKeySelector.
	 * @param BlackboardComponent Instance to access.
	 * @param BlackboardKeySelector The selector to use.
	 * @param OutDataType The out value.
	 */
	static void GetValue(const UBlackboardComponent& BlackboardComponent, const FBlackboardKeySelector& BlackboardKeySelector, typename T::FDataType& OutDataType)
	{
    	BlackboardComponent.GetValue<T>(BlackboardKeySelector.SelectedKeyName, OutDataType);
    }

	/**
	 * @brief Set the value indirectly for a UBlackboardComponent instance using a FBlackboardKeySelector.
	 * @param BlackboardComponent Instance to write.
	 * @param BlackboardKeySelector Selector to use.
	 * @param Value Value to write.
	 * @return Whether the blackboard has the key to write to.
	 */
	static bool SetValue(UBlackboardComponent& BlackboardComponent, const FBlackboardKeySelector& BlackboardKeySelector, const typename T::FDataType& Value)
	{
		return BlackboardComponent.SetValue<T>(BlackboardKeySelector.SelectedKeyName, Value);
	}

	bool ClearValue(UBlackboardComponent& BlackboardComponent) const
	{
		const FBlackboard::FKey KeyID = BlackboardComponent.GetBlackboardAsset()->GetKeyID(Name);
		if (KeyID == FBlackboard::InvalidKey)
		{
			return false;
		}
		BlackboardComponent.ClearValue(KeyID);
		return true;
	}
	
	/**
	 * @brief Populate a key selector for a blackboard asset.
	 * @param BlackboardAsset To use fot the selector.
	 * @param Owner Owner of the selector.
	 * @param PropertyName The name to use.
	 * @param OutBlackboardKeySelector The out selector.
	 */
	void MakeBlackboardKeySelector(const UBlackboardData& BlackboardAsset, UObject& Owner, const FName& PropertyName, FBlackboardKeySelector& OutBlackboardKeySelector) const
	{
	}

private:

	const FName Name;

	uint32 bInstanceSynced;
	
};

/**
 * @brief Specialization for FVectors.
 */
template
<>
inline void TNativeBlackboardKey<UBlackboardKeyType_Vector>::MakeBlackboardKeySelector(const UBlackboardData& BlackboardAsset, UObject& Owner, const FName& PropertyName, FBlackboardKeySelector& OutBlackboardKeySelector) const
{
	OutBlackboardKeySelector.AddVectorFilter(&Owner, PropertyName);
	OutBlackboardKeySelector.SelectedKeyName = Name;
	OutBlackboardKeySelector.SelectedKeyType = UBlackboardKeyType_Vector::StaticClass();
	OutBlackboardKeySelector.ResolveSelectedKey(BlackboardAsset);
}

/**
 * @brief Specialization for bool.
 */
template
<>
inline void TNativeBlackboardKey<UBlackboardKeyType_Bool>::MakeBlackboardKeySelector(const UBlackboardData& BlackboardAsset, UObject& Owner, const FName& PropertyName, FBlackboardKeySelector& OutBlackboardKeySelector) const
{
	OutBlackboardKeySelector.AddBoolFilter(&Owner, PropertyName);
	OutBlackboardKeySelector.SelectedKeyName = Name;
	OutBlackboardKeySelector.SelectedKeyType = UBlackboardKeyType_Bool::StaticClass();
	OutBlackboardKeySelector.ResolveSelectedKey(BlackboardAsset);
}

/**
 * @brief Specialization for floats.
 */
template
<>
inline void TNativeBlackboardKey<UBlackboardKeyType_Float>::MakeBlackboardKeySelector(const UBlackboardData& BlackboardAsset, UObject& Owner, const FName& PropertyName, FBlackboardKeySelector& OutBlackboardKeySelector) const
{
	OutBlackboardKeySelector.AddFloatFilter(&Owner, PropertyName);
	OutBlackboardKeySelector.SelectedKeyName = Name;
	OutBlackboardKeySelector.SelectedKeyType = UBlackboardKeyType_Float::StaticClass();
	OutBlackboardKeySelector.ResolveSelectedKey(BlackboardAsset);
}

/**
 * Provides a type safe interface over the UObject blackboard key API.
 * @tparam T the type of UObject to store in the blackboard.
 */
template
<typename T>
struct TNativeObjectBlackboardKey
{
	using Type = T;

	TNativeObjectBlackboardKey(const FName& InName, uint32 InbInstanceSynced = 1) : Name(InName), bInstanceSynced(InbInstanceSynced) {}
	
	void MakeBlackboardEntry(UBlackboardData& BlackboardData, FBlackboardEntry& OutBlackboardEntry) const
	{
		UBlackboardKeyType_Object* KeyType = NewObject<UBlackboardKeyType_Object>(&BlackboardData, Name);
		KeyType->BaseClass = T::StaticClass();
		OutBlackboardEntry.KeyType = KeyType;
		OutBlackboardEntry.EntryName = Name;
	}

	T* GetValue(const UBlackboardComponent& BlackboardComponent) const
	{
		const FBlackboard::FKey KeyID = BlackboardComponent.GetBlackboardAsset()->GetKeyID(Name);
		if (KeyID == FBlackboard::InvalidKey)
		{
			return nullptr;
		}
		UObject* Object = BlackboardComponent.GetValue<UBlackboardKeyType_Object>(KeyID);
		return Cast<T>(Object);
	}

	bool SetValue(UBlackboardComponent& BlackboardComponent, T* Object) const
	{
		const FBlackboard::FKey KeyID = BlackboardComponent.GetBlackboardAsset()->GetKeyID(Name);
		if (KeyID == FBlackboard::InvalidKey)
		{
			return false;
		}
		BlackboardComponent.SetValue<UBlackboardKeyType_Object>(KeyID, Object);
		return true;
	}

	void MakeBlackboardKeySelector(const UBlackboardData& BlackboardAsset, UBTNode& Owner, const FName& PropertyName, FBlackboardKeySelector& OutBlackboardKeySelector) const
	{
		OutBlackboardKeySelector.AddObjectFilter(&Owner, PropertyName, T::StaticClass());
		OutBlackboardKeySelector.SelectedKeyName = Name;
		OutBlackboardKeySelector.SelectedKeyType = UBlackboardKeyType_Object::StaticClass();
		OutBlackboardKeySelector.ResolveSelectedKey(BlackboardAsset);
	}

	T* GetValue(const UBlackboardComponent& BlackboardComponent, const FBlackboardKeySelector& BlackboardKeySelector) const
	{
		
		UObject* Object = BlackboardComponent.GetValue<UBlackboardKeyType_Object>(BlackboardKeySelector.SelectedKeyName);
		return Cast<T>(Object);
	}

private:

	const FName Name;

	uint32 bInstanceSynced;
	
};
