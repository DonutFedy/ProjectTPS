// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ReleaseAttack.h"
#include "../TPAIController.h"
#include "../TPCharacter.h"

UBTTask_ReleaseAttack::UBTTask_ReleaseAttack()
{
	NodeName = TEXT("ReleaseAttack");
}

EBTNodeResult::Type UBTTask_ReleaseAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ATPCharacter* ControllingPawn = Cast<ATPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn != nullptr)
	{
		ControllingPawn->ComboAttackRelease();
	}

	return EBTNodeResult::Succeeded;
}
