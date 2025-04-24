// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "AIController.h"
#include "TPAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API ATPAIController : public AAIController
{
	GENERATED_BODY()
public:
	ATPAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	void RunAI();
	void StopAI();

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
