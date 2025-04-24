// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "../TPAIController.h"
#include "../TPCharacter.h"
#include "../TPGameInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return;
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
// 	float DetectRadius = 1200.f;
// 
// 	if (World == nullptr) return;
// 	TArray<FOverlapResult> OverlapResults;
// 	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
// 	bool bResult = World->OverlapMultiByChannel(
// 		OverlapResults,
// 		Center,
// 		FQuat::Identity,
// 		ECollisionChannel::ECC_GameTraceChannel1,
// 		FCollisionShape::MakeSphere(DetectRadius),
// 		CollisionQueryParam
// 	);
// 
// 	//
// 	if (bResult)
 	{
// 		for(auto OverlapResult : OverlapResults)
		{
			//ATPCharacter* ABCharacter = Cast<ATPCharacter>(OverlapResult.GetActor());


			auto ABGameInstance = Cast<UTPGameInstance>(ControllingPawn->GetGameInstance());
			TArray<ATPCharacter*> ArrChar = ABGameInstance->GetMainCharacter();

//			if (ABCharacter && ABCharacter->GetController()->IsPlayerController())
			{
				//  [3/27/2025 ehgns] 각도 체크
// 				FVector Dir = (ABCharacter->GetActorLocation() - OwnerComp.GetAIOwner()->GetCharacter()->GetActorLocation()).GetSafeNormal();
// 				FVector ForwardDir = OwnerComp.GetAIOwner()->GetCharacter()->GetActorForwardVector();
// 				float Dot = FVector::DotProduct(ForwardDir.GetSafeNormal(), Dir);
// 				float Radian = FMath::Acos(Dot) * (180.0f / PI);
// 				if (fabs(Radian) > 40.f)
// 					continue;

				ATPCharacter* MainChar = ArrChar[0];
				TPCHECK(MainChar);

				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATPAIController::TargetKey, MainChar);
				DrawDebugSphere(World, Center, 30, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, MainChar->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), MainChar->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}


	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATPAIController::TargetKey, nullptr);
	DrawDebugSphere(World, Center, 30, 16, FColor::Red, false, 0.2f);
}