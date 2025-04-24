// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AIMoveTO2.h"
#include "../TPAIController.h"
#include "../TPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AIMoveTO2::UBTTask_AIMoveTO2()
{
	bNotifyTick = true;
	NodeName = TEXT("AI MoveTo2");
}

EBTNodeResult::Type UBTTask_AIMoveTO2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	return Result;
}

void UBTTask_AIMoveTO2::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	auto ControllingPawn = Cast<ATPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return;
	auto Target = Cast<ATPCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATPAIController::TargetKey));
	if (Target == nullptr)
		return;

	float Dist = Target->GetDistanceTo(ControllingPawn);
	bool bResult = (Dist <= ControllingPawn->GetFinalAttackRange());
	if (bResult)
	{
		float Radian = FMath::Acos(fabs(ControllingPawn->GetDotProductTo(Target))) * (180.0f / PI);
		bResult = (Radian <= ControllingPawn->GetFinalAttackAngle());
		if (bResult)
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
