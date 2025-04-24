// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillBufQuery_AddStat.h"
#include "Skills/TPSkillBase_Legacy.h"
#include "../TPCharacterStatComponent.h"

void UTPSkillBufQuery_AddStat::ProcessBuf(FTPCharacterStatInfo& InPrevStatInfo)
{
// 	float CurBaseStat = InPrevStatInfo.GetStat(CurBufStatType);
// 	RealAddStatValue = AddStatValue;
// 	InPrevStatInfo.Add_AddStat(CurBufStatType, RealAddStatValue);
}

void UTPSkillBufQuery_AddStat::UnProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo)
{
// 	InPrevStatInfo.Add_AddStat(CurBufStatType, -RealAddStatValue);
// 	RealAddStatValue = 0;
}

void UTPSkillBufQuery_AddStat::SetBufQuery(TArray< struct FTPBufStatInfo>& InSetupInfo)
{
	CurBufType = EBufType::BT_ADD_STAT;
	BufStatInfo = InSetupInfo;
}

// FString UTPSkillBufQuery_AddStat::GetTestLog()
// {
// 	FString StatType = "";
// 	switch (CurBufStatType)
// 	{
// 	case EBufStatType::BST_ATTACK:
// 		StatType = "ADD_ATTACK";
// 		break;
// 	case EBufStatType::BST_ATTACK_RATE:
// 		StatType = "ADD_ATTACK";
// 		break;
// 	case EBufStatType::BST_DEFENCE:
// 		StatType = "ADD_DEFENCE";
// 		break;
// 	case EBufStatType::BST_DEFENCE_RATE:
// 		StatType = "ADD_DEFENCE_RATE";
// 		break;
// 	case EBufStatType::BST_CRITICAL_RATE:
// 		StatType = "ADD_CRITICAL_RATE";
// 		break;
// 	case EBufStatType::BST_CRITICAL_DAMAGE:
// 		StatType = "ADD_CRITICAL_DAMAGE";
// 		break;
// 	case EBufStatType::BST_CRITICAL_DAMAGE_RATE:
// 		StatType = "ADD_CRITICAL_DAMAGE_RATE";
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	return FString::Printf(TEXT("<%s - %s> : %0.2f"), TEXT("ADD_STAT"), *StatType, RealAddStatValue);
// }
