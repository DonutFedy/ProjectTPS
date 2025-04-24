// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectTPS.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ReleaseAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UBTTask_ReleaseAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ReleaseAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
