// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillPassive_AddStat.h"

void UTPSkillPassive_AddStat::SetSkill(int InSkillIndex, int InSkillLevel, TObjectPtr<UTPSkillComponent> OwnComp)
{
	Super::SetSkill(InSkillIndex, InSkillLevel, OwnComp);

	FString AddSkillLog = "Passive Add Stat";
// 	if (CurBufBP != nullptr)
// 	{
// 		OwnComp->AddBufQuery(this, CurBufBP);
// 		AddSkillLog += " << Add Buf [" + CurBufBP->GetTestLog() + "]";
// 	}


	TPLOG(Warning, TEXT(">> Set Skill %s"), *AddSkillLog);
}

void UTPSkillPassive_AddStat::ProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo, TObjectPtr<UTPCharacterStatComponent> InStatComp)
{
	ArrRealAddStatValue.Empty();
	for (auto CurBufStat : BufStatInfo)
	{
		//float CurBaseStat = InPrevStatInfo.GetStat(CurBufStat.StatType);
		ArrRealAddStatValue.Add(CurBufStat.StatValue);
		InPrevStatInfo.Add_AddStat(CurBufStat.StatType, CurBufStat.StatValue);
		InStatComp->CallProcessBufStat(CurBufStat.StatType);
	}
	CurDurationTime = DurationTime;
}

void UTPSkillPassive_AddStat::UnProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo, TObjectPtr<UTPCharacterStatComponent> InStatComp)
{
	for (int index = 0; index <BufStatInfo.Num(); ++index)
	{
		InPrevStatInfo.Add_AddStat(BufStatInfo[index].StatType, -ArrRealAddStatValue[index]);
		InStatComp->CallProcessBufStat(BufStatInfo[index].StatType);
	}
	ArrRealAddStatValue.Empty();
}

bool UTPSkillPassive_AddStat::CheckBufTime(float DeltaTime)
{
	if (NeedDurationTime)
	{
		CurDurationTime -= DeltaTime;
		if (CurDurationTime <= 0.f)
		{
			return false;
		}
	}
	return true;
}
