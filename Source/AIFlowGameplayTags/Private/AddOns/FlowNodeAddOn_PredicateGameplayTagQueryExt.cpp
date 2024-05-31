// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "AddOns/FlowNodeAddOn_PredicateGameplayTagQueryExt.h"
#include "BlackboardKeyTypeExt_GameplayTag.h"
#include "BlackboardKeyTypeExt_GameplayTagContainer.h"
#include "BlackboardGameplayTagFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIFlowGameplayTagsLogChannels.h"
#include "Logging/LogMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNodeAddOn_PredicateGameplayTagQueryExt)

UFlowNodeAddOn_PredicateGameplayTagQueryExt::UFlowNodeAddOn_PredicateGameplayTagQueryExt()
	: Super()
{
#if WITH_EDITOR
	NodeStyle = EFlowNodeStyle::Logic;
	Category = TEXT("Blackboard");
#endif
}

#if WITH_EDITOR
void UFlowNodeAddOn_PredicateGameplayTagQueryExt::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(RF_ArchetypeObject | RF_ClassDefaultObject))
	{
		return;
	}

	if (Key.AllowedTypes.IsEmpty())
	{
		Key.AllowedTypes.Add(NewObject<UBlackboardKeyTypeExt_GameplayTag>(this));
		Key.AllowedTypes.Add(NewObject<UBlackboardKeyTypeExt_GameplayTagContainer>(this));
	}
}
#endif // WITH_EDITOR

bool UFlowNodeAddOn_PredicateGameplayTagQueryExt::EvaluatePredicate_Implementation() const
{
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!IsValid(BlackboardComponent))
	{
		UE_LOG(LogAIFlowGameplayTags, Error, TEXT("Cannot EvaluatePredicate on a blackboard key without a Blackboard"));

		return false;
	}

	UBlackboardKeyType* KeyType = GetBlackboardKeyType(*BlackboardComponent, Key.GetKeyName());
	if (!IsValidBlackboardKey(*BlackboardComponent, Key.GetKeyName()) || !IsValid(KeyType))
	{
		// Invalid Key results in a "false" result
		UE_LOG(
			LogAIFlowGameplayTags,
			Error,
			TEXT("Invalid blackboard key %s in blackboard %s"),
			*Key.GetKeyName().ToString(),
			BlackboardComponent->GetBlackboardAsset() ?
				*BlackboardComponent->GetBlackboardAsset()->GetName() :
				*BlackboardComponent->GetName());

		return false;
	}

	if (Query.IsEmpty())
	{
		// And Empty query results in a "true" result
		return true;
	}

	// Lookup the Tag or TagContainer from the blackboard
	FGameplayTagContainer TagContainerFromBlackboard;
	if (KeyType->IsA<UBlackboardKeyTypeExt_GameplayTag>())
	{
		TagContainerFromBlackboard = FGameplayTagContainer(UBlackboardGameplayTagFunctionLibrary::GetValueAsGameplayTag(BlackboardComponent, Key.GetKeyName()));
	}
	else if (KeyType->IsA<UBlackboardKeyTypeExt_GameplayTagContainer>())
	{
		TagContainerFromBlackboard = UBlackboardGameplayTagFunctionLibrary::GetValueAsGameplayTagContainer(BlackboardComponent, Key.GetKeyName());
	}
 
	// Execute the query vs. the Tag or TagContainer from the blackboard
	const bool bResult = Query.Matches(TagContainerFromBlackboard);

	return bResult;
}

void UFlowNodeAddOn_PredicateGameplayTagQueryExt::UpdateNodeConfigText_Implementation()
{
#if WITH_EDITOR
	if (!Key.GetKeyName().IsNone())
	{
		FTextBuilder TextBuilder;
		TextBuilder.AppendLine(FString::Printf(TEXT("Does %s match:"), *Key.GetKeyName().ToString()));
		TextBuilder.AppendLine(Query.GetDescription());
		SetNodeConfigText(TextBuilder.ToText());
	}
	else
	{
		SetNodeConfigText(FText());
	}
#endif // WITH_EDITOR
}