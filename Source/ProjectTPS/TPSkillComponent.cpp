// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillComponent.h"
#include "Runtime/Core/Public/Algo/Find.h"
#include "TPGameInstance.h"
#include "GamePlay/TPSkillBufQueryBase.h"
#include "GamePlay/Skills/TPSkillBase_Legacy.h"
#include "GamePlay/Skills/TPSkillController.h"

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
	ArrActiveSkills.Empty();
	ArrPassiveSkills.Empty();
	ArrBufQueries.Empty();
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
	UTPSkillController* CurSkillInfo = ABGameInstance->GetSkillController(InAddSkillInfo, this);




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

	TPLOG(Warning, TEXT("Add Skill %s"), *AddSkillLog);
}

bool UTPSkillComponent::IsHaveSkill(int SkillIndex, ESkillType InSkillType)
{
	TArray<TObjectPtr<UTPSkillBase_Legacy>>* TargetArrData = nullptr;

	switch (InSkillType)
	{
	case ESkillType::ST_PASSIVE:
		TargetArrData = &ArrPassiveSkills;
		break;
	case ESkillType::ST_ACTIVE:
		TargetArrData = &ArrActiveSkills;
		break;
	default:
		break;
	}
	if (TargetArrData == nullptr || TargetArrData->Num() <= 0)
		return false;

	TObjectPtr<UTPSkillBase_Legacy>* Found = Algo::FindByPredicate(*TargetArrData, [SkillIndex](const TObjectPtr<UTPSkillBase_Legacy>& Elem)
		{
			return Elem->SkillIndex == SkillIndex;
		});
	if (Found && Found->IsNull() == false)
	{
		return true;
	}
	else
		return false;
}

int UTPSkillComponent::GetSkillLevel(int SkillIndex, ESkillType InSkillType)
{
	TArray<TObjectPtr<UTPSkillBase_Legacy>>* TargetArrData = nullptr;

	switch (InSkillType)
	{
	case ESkillType::ST_PASSIVE:
		TargetArrData = &ArrPassiveSkills;
		break;
	case ESkillType::ST_ACTIVE:
		TargetArrData = &ArrActiveSkills;
		break;
	default:
		break;
	}
	if (TargetArrData == nullptr || TargetArrData->Num() <= 0)
		return false;

	TObjectPtr<UTPSkillBase_Legacy>* Found = Algo::FindByPredicate(*TargetArrData, [SkillIndex](const TObjectPtr<UTPSkillBase_Legacy>& Elem)
		{
			return Elem->SkillIndex == SkillIndex;
		});
	if (Found && Found->IsNull() == false)
	{
		return (*Found)->CurSkillLV;
	}
	else
		return 0;
}

float UTPSkillComponent::GetSkillColdTime(int SkillIndex)
{
	if (ArrActiveSkills.Num() <= 0)
		return 0.f;

	TObjectPtr<UTPSkillBase_Legacy>* Found = Algo::FindByPredicate(ArrActiveSkills, [SkillIndex](const TObjectPtr<UTPSkillBase_Legacy>& Elem)
		{
			return Elem->SkillIndex == SkillIndex;
		});
	if (Found && Found->IsNull() == false)
	{
		return (*Found)->CurrentColdTime;
	}
	else
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
	FTPCharacterStatInfo& CurAddStat = spStatComp->GetStatInfo();
	for (int PassiveIndex = 0; PassiveIndex < ArrPassiveSkills.Num(); )
	{
		if (ArrPassiveSkills[PassiveIndex]->CheckBufTime(DeltaTime) == false)
		{
			ArrPassiveSkills[PassiveIndex]->UnProcessBuf(CurAddStat, spStatComp);
			ArrPassiveSkills.RemoveAt(PassiveIndex);
			continue;
		}
		++PassiveIndex;
	}
}

void UTPSkillComponent::AddBufQuery(TObjectPtr<class UTPSkillBase_Legacy> OwnSkill)
{
	FTPCharacterStatInfo& CurAddStat = spStatComp->GetStatInfo();
	int FindIndex = ArrBufQueries.Find(OwnSkill);
	if (FindIndex != INDEX_NONE)
	{
		ArrBufQueries[FindIndex]->UnProcessBuf(CurAddStat, spStatComp);
	}
	else
	{
		ArrBufQueries.Add(OwnSkill);
	}
	if (OwnSkill)
	{
		OwnSkill->ProcessBuf(CurAddStat, spStatComp);
	}
}

// Called every frame
void UTPSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

