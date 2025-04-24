// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AIMoveTo.h"
#include "../TPAIController.h"
#include "../TPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AIMoveTo::UBTTask_AIMoveTo()
{
	NodeName = TEXT("AI MoveTo");
}

EBTNodeResult::Type UBTTask_AIMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ATPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return EBTNodeResult::Type::Failed;
	auto Target = Cast<ATPCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATPAIController::TargetKey));
	if (Target == nullptr)
		return EBTNodeResult::Type::Failed;

	float Dist = Target->GetDistanceTo(ControllingPawn);
	bool bResult = (Dist <= ControllingPawn->GetFinalAttackRange());
	if (bResult)
	{
		return EBTNodeResult::Type::Failed;
	}

	return Result;
}
