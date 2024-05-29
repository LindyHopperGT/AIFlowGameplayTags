// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "AddOns/AIFlowNodeAddOn.h"
#include "Interfaces/FlowPredicateInterface.h"
#include "Types/FlowBlackboardEntry.h"

#include "GameplayTagContainer.h"

#include "FlowNodeAddOn_PredicateGameplayTagQueryExt.generated.h"

// Forward Declarations

UCLASS(MinimalApi, NotBlueprintable, meta = (DisplayName = "Gameplay Tag Query"))
class UFlowNodeAddOn_PredicateGameplayTagQueryExt
	: public UAIFlowNodeAddOn
	, public IFlowPredicateInterface
{
	GENERATED_BODY()

public:

	UFlowNodeAddOn_PredicateGameplayTagQueryExt();

	// IFlowPredicateInterface
	virtual bool EvaluatePredicate_Implementation() const override;
	// --

#if WITH_EDITOR
	// UObject Interface
	virtual void PostInitProperties() override;
	// --
#endif

	// UFlowNodeBase
	virtual void UpdateNodeConfigText_Implementation() override;
	// --

public:

	// Blackboard key for the Gameplay Tag or Tag Container to test with the Query
	UPROPERTY(EditAnywhere, Category = Configuration, DisplayName = "Key")
	FFlowBlackboardEntry Key;

	// Query to evaluate the blackboard entry with
	UPROPERTY(EditAnywhere, Category = Configuration, DisplayName = "Query")
	FGameplayTagQuery Query;
};
