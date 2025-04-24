// Fill out your copyright notice in the Description page of Project Settings.


#include "TPAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ATPAIController::HomePosKey(TEXT("HomePos"));
const FName ATPAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ATPAIController::TargetKey(TEXT("Target"));

//////////////////////// Version 2 /////////////////////////
ATPAIController::ATPAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_TPCharacter.BB_TPCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_TPCharacter.BT_TPCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ATPAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ATPAIController::RunAI()
{
	UBlackboardComponent* CurBC = GetBlackboardComponent();
	if (UseBlackboard(BBAsset, CurBC))
	{
		APawn* CurPawn = GetPawn();
		Blackboard->SetValueAsVector(HomePosKey, CurPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			TPLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void ATPAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent != nullptr)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}


