// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_AIMoveTO2.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UBTTask_AIMoveTO2 : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:

	UBTTask_AIMoveTO2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
