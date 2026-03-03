// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#include "AddOns/FlowNodeAddOn_PredicateRequireGameplayTagsBlackboard.h"
#include "AIFlowGameplayTagsLogChannels.h"
#include "BlackboardKeyTypeExt_GameplayTag.h"
#include "BlackboardKeyTypeExt_GameplayTagContainer.h"
#include "BlackboardGameplayTagFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Logging/LogMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(FlowNodeAddOn_PredicateRequireGameplayTagsBlackboard)

UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard::UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard()
	: Super()
{
#if WITH_EDITOR
	NodeDisplayStyle = FlowNodeStyle::AddOn_Predicate;
	Category = TEXT("Blackboard");
#endif
}

#if WITH_EDITOR
void UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard::PostInitProperties()
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

void UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UpdateNodeConfigText();
}

void UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard::OnEditorPinConnectionsChanged(const TArray<FFlowPinConnectionChange>& Changes)
{
	Super::OnEditorPinConnectionsChanged(Changes);

	UpdateNodeConfigText();
}

#endif

bool UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard::EvaluatePredicate_Implementation() const
{
	if (Requirements.IsEmpty())
	{
		// And Empty Requirements results in a "true" result
		return true;
	}

	// Sourcing the tags from the blackboard
	FGameplayTagContainer TagsToTestValue;
	if (!TryGetTagsToCheckFromBlackboard(TagsToTestValue))
	{
		return false;
	}

	// Execute the Requirements vs. TagContainer
	const bool bResult = Requirements.RequirementsMet(TagsToTestValue);
	return bResult;
}

bool UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard::TryGetTagsToCheckFromBlackboard(FGameplayTagContainer& TagsToCheckValue) const
{
	if (!Key.KeyName.IsNone())
	{
		return false;
	}

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

	// Lookup the Tag or TagContainer from the blackboard
	if (KeyType->IsA<UBlackboardKeyTypeExt_GameplayTag>())
	{
		TagsToCheckValue = FGameplayTagContainer(UBlackboardGameplayTagFunctionLibrary::GetValueAsGameplayTag(BlackboardComponent, Key.GetKeyName()));
	}
	else if (KeyType->IsA<UBlackboardKeyTypeExt_GameplayTagContainer>())
	{
		TagsToCheckValue = UBlackboardGameplayTagFunctionLibrary::GetValueAsGameplayTagContainer(BlackboardComponent, Key.GetKeyName());
	}

	return true;
}

void UFlowNodeAddOn_PredicateRequireGameplayTagsBlackboard::UpdateNodeConfigText_Implementation()
{
#if WITH_EDITOR
	FTextBuilder TextBuilder;
	if (Requirements.IsEmpty())
	{
		const FName RequirementsName = GET_MEMBER_NAME_CHECKED(ThisClass, Requirements);
		TextBuilder.AppendLine(FString::Printf(TEXT("<not configured - Must have configured %s>"), *RequirementsName.ToString()));
	}
	else if (!Key.GetKeyName().IsNone())
	{
		TextBuilder.AppendLine(FString::Printf(TEXT("<not configured - must have a blackboard key>")));
	}
	else
	{
		TextBuilder.AppendLine(FString::Printf(TEXT("Does %s match:"), *Key.GetKeyName().ToString()));
		TextBuilder.AppendLine(Requirements.ToString());
	}

	SetNodeConfigText(TextBuilder.ToText());
#endif // WITH_EDITOR
}