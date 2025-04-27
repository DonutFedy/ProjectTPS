// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillController.h"
#include "../../Management/TPStageManager.h"
#include "TPSkillBase.h"
#include "../../Table/TPSkillTable.h"
#include "../../TPSkillComponent.h"
#include "Passive/TPPassiveBase.h"
#include "Passive/TPPassive_AddStat.h"


void UTPSkillController::InitSkill(FTPSkillInitData& InitInfo, UTPSkillComponent* SkillComp, UTPStageManager* StageMgr)
{
	CurSetupInfo = InitInfo;
	FTPSkillTable* CurSkillInfo = StageMgr->GetSkillInfo(InitInfo.SkillIndex);
	TPCHECK(CurSkillInfo != nullptr);

	switch (CurSkillInfo->SkillType)
	{
		case ESkillType::ST_PASSIVE:
		{
			// ¿ø·¡´Â ¹ºÁö ±¸ºÐÇØ¾ßµÊ.
			ArrCurSkillObj = GetPassiveObj(InitInfo.SkillLv, StageMgr, CurSkillInfo);
		}
			break;
		default:
			break;
	}
	TPCHECK(ArrCurSkillObj.Num()>0 );
	for(auto CurSkill : ArrCurSkillObj)
		CurSkill->InitSkill(InitInfo.SkillLv, *CurSkillInfo, SkillComp, StageMgr);
}

void UTPSkillController::ReleaseSkill()
{
	for (auto CurSkillObj : ArrCurSkillObj)
	{
		CurSkillObj->ReleaseSkill();
	}
	ArrCurSkillObj.Empty();
}

void UTPSkillController::CheckSkillCondition(float DeltaTime)
{
	TPCHECK(ArrCurSkillObj.Num() > 0);

	for (auto CurSkill : ArrCurSkillObj)
		CurSkill->CheckSkillCondition(DeltaTime);
}

void UTPSkillController::CheckSkillConditionAfterAction(ESkillConditionType ActionType)
{
	TPCHECK(ArrCurSkillObj.Num() > 0);

	for (auto CurSkill : ArrCurSkillObj)
		CurSkill->CheckSkillConditionAfterAction(ActionType);
}

FString UTPSkillController::_GetSkillLog()
{
	return FString::Printf(TEXT("Idx : %d, Lv : %d"), CurSetupInfo.SkillIndex ,CurSetupInfo.SkillLv);
}

TArray<TObjectPtr<UTPSkillBase>> UTPSkillController::GetPassiveObj(int InSkillLV, UTPStageManager* StageMgr, FTPSkillTable* Info)
{
	TArray<TObjectPtr<UTPSkillBase>> ArrResult;
	TPCHECK(StageMgr != nullptr, ArrResult);
	TArray<FTPPassiveGroupTable*> ArrCurPassiveGroupInfo = StageMgr->GetPassiveSkillInfo(Info->GroupID, InSkillLV);
	TArray<ESkillEffectType> ArrNeedEffectType;

	for (auto CurInfo : ArrCurPassiveGroupInfo)
	{
		if(ArrNeedEffectType.Find(CurInfo->EffectType)== INDEX_NONE)
			ArrNeedEffectType.Add(CurInfo->EffectType);
	}

	TPCHECK(ArrNeedEffectType.Num()>0, ArrResult);

	for (auto NeedCurType : ArrNeedEffectType)
	{
		switch (NeedCurType)
		{
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_SHIELDMAX:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_HPMAX:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_STAMINAMAX:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_ATK:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_DEF:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_MOVESPD:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_CRITRATE:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_RELOAD:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_BULLETSPD:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_ACCURACY:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_RECOIL:
		case ESkillEffectType::SEffect_ADD_DODGE_INVINSIBLE:
		case ESkillEffectType::SEffect_MULTIPLY_DODGE_STAMINA:
		case ESkillEffectType::SEffect_MULTIPLY_WEAPON_STAT_MAXAMMO:
		case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_SKILLCOOLTIME:
		case ESkillEffectType::SEffect_ADD_ACTIVE_STAT_USECOUNT:
			ArrResult.Add( NewObject<UTPPassive_AddStat>());
			break;
		default:
			break;
		}
	}
	return ArrResult;
}
