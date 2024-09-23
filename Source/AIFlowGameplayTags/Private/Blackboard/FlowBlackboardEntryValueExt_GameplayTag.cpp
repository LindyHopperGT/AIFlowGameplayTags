// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "Blackboard/FlowBlackboardEntryValueExt_GameplayTag.h"
#include "BlackboardGameplayTagFunctionLibrary.h"
#include "BlackboardKeyTypeExt_GameplayTag.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIFlowGameplayTagsLogChannels.h"
#include "Logging/LogMacros.h"
#include "Nodes/FlowNode.h"
#include "Types/FlowDataPinProperties.h"
#include "Types/FlowDataPinResults.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowBlackboardEntryValueExt_GameplayTag)

#if WITH_EDITOR
void UFlowBlackboardEntryValueExt_GameplayTag::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ArchetypeObject | RF_ClassDefaultObject))
	{
		if (Key.AllowedTypes.IsEmpty())
		{
			Key.AllowedTypes.Add(NewObject<UBlackboardKeyTypeExt_GameplayTag>(this));
		}
	}
}

FString UFlowBlackboardEntryValueExt_GameplayTag::GetEditorValueString() const
{
	return TagValue.ToString();
}

FText UFlowBlackboardEntryValueExt_GameplayTag::BuildNodeConfigText() const
{
	return FText::FromString(FString::Printf(TEXT("Set %s to %s"), *Key.GetKeyName().ToString(), *GetEditorValueString()));
}
#endif // WITH_EDITOR

bool UFlowBlackboardEntryValueExt_GameplayTag::TryProvideFlowDataPinProperty(const bool bIsInputPin, TInstancedStruct<FFlowDataPinProperty>& OutFlowDataPinProperty) const
{
	if (bIsInputPin)
	{
		OutFlowDataPinProperty.InitializeAs<FFlowDataPinInputProperty_GameplayTag>(TagValue);
	}
	else
	{
		OutFlowDataPinProperty.InitializeAs<FFlowDataPinOutputProperty_GameplayTag>(TagValue);
	}

	return true;
}

bool UFlowBlackboardEntryValueExt_GameplayTag::TrySetValueFromInputDataPin(const FName& PinName, UFlowNode& PinOwnerFlowNode)
{
	const FFlowDataPinResult_GameplayTag FlowDataPinResult = PinOwnerFlowNode.TryResolveDataPinAsGameplayTag(PinName);

	if (FlowDataPinResult.Result == EFlowDataPinResolveResult::Success)
	{
		TagValue = FlowDataPinResult.Value;

		return true;
	}

	return false;
}

void UFlowBlackboardEntryValueExt_GameplayTag::SetOnBlackboardComponent(UBlackboardComponent* BlackboardComponent) const
{
	if (IsValid(BlackboardComponent))
	{
		UBlackboardGameplayTagFunctionLibrary::SetValueAsGameplayTag(BlackboardComponent, Key.GetKeyName(), TagValue);
	}
}

EBlackboardCompare::Type UFlowBlackboardEntryValueExt_GameplayTag::CompareKeyValues(const UBlackboardComponent* BlackboardComponent, const FName& OtherKeyName) const
{
	if (!IsValid(BlackboardComponent))
	{
		UE_LOG(LogAIFlowGameplayTags, Error, TEXT("Cannot CompareKeyValues without a Blackboard!"));
		return EBlackboardCompare::NotEqual;
	}

	const FGameplayTag OtherValueAsTag = UBlackboardGameplayTagFunctionLibrary::GetValueAsGameplayTag(BlackboardComponent, OtherKeyName);

	if (TagValue == OtherValueAsTag)
	{
		return EBlackboardCompare::Equal;
	}
	else
	{
		return EBlackboardCompare::NotEqual;
	}
}

TSubclassOf<UBlackboardKeyType> UFlowBlackboardEntryValueExt_GameplayTag::GetSupportedBlackboardKeyType() const
{
	return UBlackboardKeyTypeExt_GameplayTag::StaticClass();
}
