// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillBufQuery_MulStat.h"
#include "../TPCharacterStatComponent.h"

void UTPSkillBufQuery_MulStat::ProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo)
{
	float CurBaseStat = InPrevStatInfo.GetStat(CurBufStatType);
	RealAddStatValue = CurBaseStat* MulStatValue - CurBaseStat;
	InPrevStatInfo.Add_AddStat(CurBufStatType, RealAddStatValue);

	//InPrevStatInfo.Mul_AddStat(CurBufStatType, MulStatValue);
}

void UTPSkillBufQuery_MulStat::UnProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo)
{
	InPrevStatInfo.Add_AddStat(CurBufStatType, -RealAddStatValue);
	RealAddStatValue = 0;
}

void UTPSkillBufQuery_MulStat::SetBufQuery(EBufStatType InStatType, float InMulStatValue)
{
	CurBufType = EBufType::BT_MUL_STAT;
	CurBufStatType = InStatType;
	MulStatValue = InMulStatValue;
}

FString UTPSkillBufQuery_MulStat::GetTestLog()
{
	FString StatType = "";
	switch (CurBufStatType)
	{
	case EBufStatType::BST_ATTACK:
		StatType = "ADD_ATTACK";
		break;
	case EBufStatType::BST_ATTACK_RATE:
		StatType = "ADD_ATTACK";
		break;
	case EBufStatType::BST_DEFENCE:
		StatType = "ADD_DEFENCE";
		break;
	case EBufStatType::BST_DEFENCE_RATE:
		StatType = "ADD_DEFENCE_RATE";
		break;
	case EBufStatType::BST_CRITICAL_RATE:
		StatType = "ADD_CRITICAL_RATE";
		break;
	case EBufStatType::BST_CRITICAL_DAMAGE:
		StatType = "ADD_CRITICAL_DAMAGE";
		break;
	case EBufStatType::BST_CRITICAL_DAMAGE_RATE:
		StatType = "ADD_CRITICAL_DAMAGE_RATE";
		break;
	default:
		break;
	}

	return FString::Printf(TEXT("<%s - %s> : %0.2f"), TEXT("MUL_STAT"), *StatType, RealAddStatValue);
}
