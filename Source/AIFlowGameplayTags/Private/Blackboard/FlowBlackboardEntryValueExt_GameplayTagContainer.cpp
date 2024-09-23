// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Blackboard/FlowBlackboardEntryValueExt_GameplayTagContainer.h"
#include "BlackboardGameplayTagFunctionLibrary.h"
#include "BlackboardKeyTypeExt_GameplayTagContainer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIFlowGameplayTagsLogChannels.h"
#include "Logging/LogMacros.h"
#include "Nodes/FlowNode.h"
#include "Types/FlowDataPinProperties.h"
#include "Types/FlowDataPinResults.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowBlackboardEntryValueExt_GameplayTagContainer)

#if WITH_EDITOR
void UFlowBlackboardEntryValueExt_GameplayTagContainer::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ArchetypeObject | RF_ClassDefaultObject))
	{
		if (Key.AllowedTypes.IsEmpty())
		{
			Key.AllowedTypes.Add(NewObject<UBlackboardKeyTypeExt_GameplayTagContainer>(this));
		}
	}
}

FString UFlowBlackboardEntryValueExt_GameplayTagContainer::GetEditorValueString() const
{
	return TagContainerValue.ToStringSimple();
}

FText UFlowBlackboardEntryValueExt_GameplayTagContainer::BuildNodeConfigText() const
{
	return FText::FromString(FString::Printf(TEXT("Set %s to %s"), *Key.GetKeyName().ToString(), *GetEditorValueString()));
}
#endif // WITH_EDITOR

bool UFlowBlackboardEntryValueExt_GameplayTagContainer::TryProvideFlowDataPinProperty(const bool bIsInputPin, TInstancedStruct<FFlowDataPinProperty>& OutFlowDataPinProperty) const
{
	if (bIsInputPin)
	{
		OutFlowDataPinProperty.InitializeAs<FFlowDataPinInputProperty_GameplayTagContainer>(TagContainerValue);
	}
	else
	{
		OutFlowDataPinProperty.InitializeAs<FFlowDataPinOutputProperty_GameplayTagContainer>(TagContainerValue);
	}

	return true;
}

bool UFlowBlackboardEntryValueExt_GameplayTagContainer::TrySetValueFromInputDataPin(const FName& PinName, UFlowNode& PinOwnerFlowNode)
{
	const FFlowDataPinResult_GameplayTagContainer FlowDataPinResult = PinOwnerFlowNode.TryResolveDataPinAsGameplayTagContainer(PinName);

	if (FlowDataPinResult.Result == EFlowDataPinResolveResult::Success)
	{
		TagContainerValue = FlowDataPinResult.Value;

		return true;
	}

	return false;
}

void UFlowBlackboardEntryValueExt_GameplayTagContainer::SetOnBlackboardComponent(UBlackboardComponent* BlackboardComponent) const
{
	if (IsValid(BlackboardComponent))
	{
		UBlackboardGameplayTagFunctionLibrary::SetValueAsGameplayTagContainer(BlackboardComponent, Key.GetKeyName(), TagContainerValue);
	}
}

EBlackboardCompare::Type UFlowBlackboardEntryValueExt_GameplayTagContainer::CompareKeyValues(const UBlackboardComponent* BlackboardComponent, const FName& OtherKeyName) const
{
	if (!IsValid(BlackboardComponent))
	{
		UE_LOG(LogAIFlowGameplayTags, Error, TEXT("Cannot CompareKeyValues without a Blackboard!"));
		return EBlackboardCompare::NotEqual;
	}

	const FGameplayTagContainer OtherValueAsTagContainer = UBlackboardGameplayTagFunctionLibrary::GetValueAsGameplayTagContainer(BlackboardComponent, OtherKeyName);

	if (TagContainerValue == OtherValueAsTagContainer)
	{
		return EBlackboardCompare::Equal;
	}
	else
	{
		return EBlackboardCompare::NotEqual;
	}
}

TSubclassOf<UBlackboardKeyType> UFlowBlackboardEntryValueExt_GameplayTagContainer::GetSupportedBlackboardKeyType() const
{
	return UBlackboardKeyTypeExt_GameplayTagContainer::StaticClass();
}
