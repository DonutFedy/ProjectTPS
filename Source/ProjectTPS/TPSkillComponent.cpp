// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillComponent.h"
#include "Runtime/Core/Public/Algo/Find.h"
#include "TPGameInstance.h"
#include "GamePlay/TPSkillBufQueryBase.h"
#include "GamePlay/Skills/TPSkillBase_Legacy.h"
#include "GamePlay/Skills/TPSkillController.h"
#include "Table/TPSkillTable.h"
#include "GamePlay/Skills/Buf/TPBufBase.h"
#include <Runtime/Engine/Classes/GameFramework/Actor.h>
#include "TPCharacterStatComponent.h"
#include "TPCharacter.h"

// Sets default values for this component's properties
UTPSkillComponent::UTPSkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


	bWantsInitializeComponent = true;
	// ...
}

void UTPSkillComponent::SetSkillComponent(TObjectPtr <UTPCharacterStatComponent> InCharStateComp, TArray<FTPSkillInitData>& InStartSkills)
{
	spStatComp = InCharStateComp;

	// 해당 데이터를 로드한다....
	for (auto CurAddSkill : InStartSkills)
	{
		AddSkill(CurAddSkill);
	}
}

void UTPSkillComponent::AddSkill(FTPSkillInitData& InAddSkillInfo)
{
	FString AddSkillLog = "";
	// 해당 스킬을 추가한다.
 	auto ABGameInstance = Cast<UTPGameInstance>(GetWorld()->GetGameInstance());
	UTPSkillController* CurSkillController = ABGameInstance->GetSkillController(InAddSkillInfo, this);
	TPCHECK(CurSkillController != nullptr);

	// 스킬 관리자에 넘기기.
	ArrSkillController.Add(CurSkillController);


// 	UTPSkillBase_Legacy* CurSkillInfo = ABGameInstance->GetSkillInfo_Legacy(InAddSkillInfo);
// 
// 	// 만약 active 스킬이라면
// 	if (InAddSkillInfo.SkillType == ESkillType::ST_ACTIVE)
// 	{
// 		AddSkillLog += "[ACTIVE] \n";
// 		ArrActiveSkills.Add(CurSkillInfo);
// 	}
// 	// 만약 passive 스킬이라면 바로 적용
// 	else if (InAddSkillInfo.SkillType == ESkillType::ST_PASSIVE)
// 	{
// 		AddSkillLog += "[PASSIVE] \n";
// 		ArrPassiveSkills.Add(CurSkillInfo);
// 	}
// 	CurSkillInfo->SetSkill(InAddSkillInfo.SkillIndex, InAddSkillInfo.SkillLv, this);

	TPLOG(Warning, TEXT("Add Skill %d"), InAddSkillInfo.SkillIndex);
}


void UTPSkillComponent::RemoveSkill(int RemoveSkillIndex)
{
	FString AddSkillLog = "";
	// 스킬 관리자에서 제거.
	for (auto FindSkillInfo : ArrSkillController)
	{
		if (FindSkillInfo->GetSkillIndex() != RemoveSkillIndex)
			continue;
		FindSkillInfo->ReleaseSkill();
		ArrSkillController.Remove(FindSkillInfo);
		break;
	}

	TPLOG(Warning, TEXT("Remove Skill %d"), RemoveSkillIndex);
}

bool UTPSkillComponent::IsHaveSkill(int SkillIndex, ESkillType InSkillType)
{
	return false;
}

int UTPSkillComponent::GetSkillLevel(int SkillIndex, ESkillType InSkillType)
{
	return 0;
}

float UTPSkillComponent::GetSkillColdTime(int SkillIndex)
{
	return 0;
}

// Called when the game starts
void UTPSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



void UTPSkillComponent::TickSkillComponent(float DeltaTime)
{
// 	for (int PassiveIndex = 0; PassiveIndex < ArrPassiveSkills.Num(); )
// 	{
// 		if (ArrPassiveSkills[PassiveIndex]->CheckBufTime(DeltaTime) == false)
// 		{
// 			ArrPassiveSkills[PassiveIndex]->UnProcessBuf(CurAddStat, spStatComp);
// 			ArrPassiveSkills.RemoveAt(PassiveIndex);
// 			continue;
// 		}
// 		++PassiveIndex;
// 	}

	for (auto SkillCon : ArrSkillController)
	{
		if (SkillCon)
		{
			SkillCon->CheckSkillCondition(DeltaTime);
		}
	}
	for (auto BufEffects : MapBuf)
	{
		if (BufEffects.Value != nullptr)
		{
			BufEffects.Value->CheckBufTime(DeltaTime);
		}
	}
	_TestLog(DeltaTime);
}

void UTPSkillComponent::_TestLog(float DeltaTime)
{
	TPCHECK(spStatComp!=nullptr);

	FVector vLogPos = spStatComp->GetOwnChar()->GetActorLocation() + spStatComp->GetOwnChar()->GetActorRightVector() * -100 + FVector(0.f, 0.f, 100.f);

	FString strAllLog = "";
	for (auto CurSkill : ArrSkillController)
	{
		strAllLog+=(CurSkill->_GetSkillLog()+"\n");
	}

	DrawDebugString(
		GetWorld(),
		vLogPos,          // 표시할 위치
		*strAllLog,           // 표시할 텍스트
		nullptr,                         // 소유 액터 (없으면 nullptr)
		FColor::Red,                    // 텍스트 색상
		DeltaTime,                            // 지속 시간
		true                             // 깊이 테스트 여부 (false면 벽 뒤에서도 보임)
	);

}

bool UTPSkillComponent::AddBuf(int SkillSerializeIndex, FTPPassiveGroupTable& NewBufInfo)
{
	// 버프 세팅
	if (MapBuf.Find(SkillSerializeIndex))
	{
		// 기존 버프에 추가한다.
		MapBuf[SkillSerializeIndex]->AddEffect(NewBufInfo);
		return true;
	}
	else
	{
		TObjectPtr<UTPBufBase> NewBuf = NewObject<UTPBufBase>();
		NewBuf->InitBuf(NewBufInfo, this);
		// 버프 등록
		MapBuf.Add(SkillSerializeIndex, NewBuf);
		return true;
	}
	return false;
}

void UTPSkillComponent::RemoveEffect(int SkillSerializeIndex, ESkillEffectType InRemoveTargetType)
{
	if (MapBuf.Find(SkillSerializeIndex) == nullptr)
		return;
	if(MapBuf[SkillSerializeIndex]->RemoveEffect(InRemoveTargetType))
		MapBuf.Remove(SkillSerializeIndex);
}

// Called every frame
void UTPSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

