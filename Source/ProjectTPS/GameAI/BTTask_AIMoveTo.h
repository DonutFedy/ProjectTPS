// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UBTTask_AIMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_AIMoveTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
