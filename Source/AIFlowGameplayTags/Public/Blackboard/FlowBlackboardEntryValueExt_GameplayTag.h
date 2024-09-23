// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "Blackboard/FlowBlackboardEntryValue.h"

#include "GameplayTagContainer.h"

#include "FlowBlackboardEntryValueExt_GameplayTag.generated.h"

/**
 * Configuration object for setting blackboard entries for UBlackboardKeyTypeExt_GameplayTag entries
 */
UCLASS(BlueprintType, DisplayName = "GameplayTag Blackboard Value")
class AIFLOWGAMEPLAYTAGS_API UFlowBlackboardEntryValueExt_GameplayTag : public UFlowBlackboardEntryValue
{
	GENERATED_BODY()

public:

	// UFlowBlackboardEntryValue
	virtual void SetOnBlackboardComponent(UBlackboardComponent* BlackboardComponent) const override;
	virtual EBlackboardCompare::Type CompareKeyValues(const UBlackboardComponent* BlackboardComponent, const FName& OtherKeyName) const override;
	virtual TSubclassOf<UBlackboardKeyType> GetSupportedBlackboardKeyType() const override;
	virtual bool TrySetValueFromInputDataPin(const FName& PinName, UFlowNode& PinOwnerFlowNode) override;
#if WITH_EDITOR
	virtual FString GetEditorValueString() const override;
#endif // WITH_EDITOR
	// --
	
	// IFlowDataPinPropertyProviderInterface
	virtual bool TryProvideFlowDataPinProperty(const bool bIsInputPin, TInstancedStruct<FFlowDataPinProperty>& OutFlowDataPinProperty) const override;
	// --

#if WITH_EDITOR
public:
	// UFlowNodeBase
	virtual FText BuildNodeConfigText() const override;
	// --

	// UObject
	virtual void PostInitProperties() override;
	// --
#endif // WITH_EDITOR

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Configuration, meta = (DisplayAfter = Key))
	FGameplayTag TagValue;
};
