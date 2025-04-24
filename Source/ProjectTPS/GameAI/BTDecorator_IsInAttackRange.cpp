// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "../TPAIController.h"
#include "../TPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ATPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
		return false;

	auto Target = Cast<ATPCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATPAIController::TargetKey));
	if (Target == nullptr)
		return false;

	float Dist =Target->GetDistanceTo(ControllingPawn);
	bResult = (Dist <= ControllingPawn->GetFinalAttackRange());
	if (bResult)
	{
		float Radian = FMath::Acos(fabs(ControllingPawn->GetDotProductTo(Target))) * (180.0f / PI);
		bResult = (Radian <= ControllingPawn->GetFinalAttackAngle());
	}
	return bResult;
}