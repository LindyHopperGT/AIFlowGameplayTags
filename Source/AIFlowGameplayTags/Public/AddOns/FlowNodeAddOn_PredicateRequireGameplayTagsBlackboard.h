// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors
#pragma once

#include "AddOns/AIFlowNodeAddOn.h"
#include "Interfaces/FlowPredicateInterface.h"
#include "Types/FlowBlackboardEntry.h"

#include "GameplayEffectTypes.h"

#include "FlowNodeAddOn_PredicateRequireGameplayTagsBlackboard.generated.h"

UCLASS(MinimalApi, NotBlueprintable, meta = (DisplayName = "Require Gameplay Tags (blackboard)"))
class UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard
	: public UAIFlowNodeAddOn
	, public IFlowPredicateInterface
{
	GENERATED_BODY()

public:

	UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard();

	// IFlowPredicateInterface
	virtual bool EvaluatePredicate_Implementation() const override;
	// --

#if WITH_EDITOR
	// UObject Interface
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	// --

	// UFlowNodeBase
	virtual void OnEditorPinConnectionsChanged(const TArray<FFlowPinConnectionChange>& Changes) override;
	// --
#endif

	// UFlowNodeBase
	virtual void UpdateNodeConfigText_Implementation() override;
	// --

	bool TryGetTagsToCheckFromBlackboard(FGameplayTagContainer& TagsToCheckValue) const;

public:

	// Blackboard key for the Gameplay Tag or Tag Container to test with the Requirements
	UPROPERTY(EditAnywhere, Category = Configuration, DisplayName = "Key")
	FFlowBlackboardEntry Key;

	// Requirements to evaluate the blackboard entry with
	UPROPERTY(EditAnywhere, Category = Configuration, DisplayName = "Requirements")
	FGameplayTagRequirements Requirements;
};