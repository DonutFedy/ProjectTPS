// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillController.h"
#include "../../Management/TPStageManager.h"
#include "TPSkillBase.h"
#include "../../Table/TPSkillTable.h"
#include "../../TPSkillComponent.h"
#include "Passive/TPPassiveBase.h"
#include "Passive/TPPassive_AddStat.h"
#include "Passive/TPPassive_Bullet.h"
#include "Active/TPActive_Granade.h"
#include "Active/TPActive_AttackBuf.h"
#include "Active/TPActive_Heal.h"


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
		case ESkillType::ST_ACTIVE:
		{
			// ¿ø·¡´Â ¹ºÁö ±¸ºÐÇØ¾ßµÊ.
			ArrCurSkillObj = GetActiveObj(InitInfo.SkillLv, StageMgr, CurSkillInfo);
		}
			break;
		default:
			break;
	}
	SetSkillType(CurSkillInfo->SkillType);
	CurSetupInfo.SkillUseType = CurSkillInfo->SkillUseType;

	TPCHECK(ArrCurSkillObj.Num()>0 );
	for(auto CurSkill : ArrCurSkillObj)
		CurSkill->InitSkill(this, InitInfo.SkillLv, *CurSkillInfo, SkillComp, StageMgr);
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

bool UTPSkillController::CheckSkillConditionAfterAction(ESkillConditionType ActionType)
{
	TPCHECK(ArrCurSkillObj.Num() > 0, false);
	bool result = false;
	for (auto CurSkill : ArrCurSkillObj)
	{
		bool curResult = CurSkill->CheckSkillConditionAfterAction(ActionType);
		if(result == false)
			result = curResult;
	}

	return result;
}

int UTPSkillController::GetEffectValue(ESkillEffectType InEffectType)
{
	for (auto CurSkill : ArrCurSkillObj)
	{
		CurSkill->GetEffectValue(InEffectType);
	}
	return INDEX_NONE;
}

bool UTPSkillController::ReleaseActiveSkill()
{
	TPCHECK(ArrCurSkillObj.Num() > 0, false);
	bool result = false;
	for (auto CurSkill : ArrCurSkillObj)
	{
		bool curResult = CurSkill->ReleaseActiveSkill();
		if (result == false)
			result = curResult;
	}
	return result;
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
		case ESkillEffectType::SEffect_ADD_BOUNCE_BULLET:
			ArrResult.Add( NewObject<UTPPassive_AddStat>());
			break;
		case ESkillEffectType::SEffect_ADD_FIRE_BULLET_NUM:
			ArrResult.Add( NewObject<UTPPassive_Bullet>());
			break;
		default:
			break;
		}
	}
	return ArrResult;
}

TArray<TObjectPtr<UTPSkillBase>> UTPSkillController::GetActiveObj(int InSkillLV, class UTPStageManager* StageMgr, struct FTPSkillTable* Info)
{
	TArray<TObjectPtr<UTPSkillBase>> ArrResult;
	TPCHECK(StageMgr != nullptr, ArrResult);
	TArray<FTPActiveGroupTable*> ArrCurActiveGroupInfo = StageMgr->GetActiveSkillInfo(Info->GroupID, InSkillLV);
	TArray<ESkillEffectType> ArrNeedEffectType;

	for (auto CurInfo : ArrCurActiveGroupInfo)
	{
		if (ArrNeedEffectType.Find(CurInfo->EffectType) == INDEX_NONE)
			ArrNeedEffectType.Add(CurInfo->EffectType);
	}

	TPCHECK(ArrNeedEffectType.Num() > 0, ArrResult);

	for (auto NeedCurType : ArrNeedEffectType)
	{
		switch (NeedCurType)
		{
		case ESkillEffectType::SEffect_TAKE_DMG:
			ArrResult.Add(NewObject<UTPActive_Granade>());
			break;
		case ESkillEffectType::SEffect_ADD_WEAPON_ATK:
			ArrResult.Add(NewObject<UTPActive_AttackBuf>());
			break;
		case ESkillEffectType::SEffect_ADD_CURRENT_HP:
			ArrResult.Add(NewObject<UTPActive_Heal>());
			break;
		default:
			break;
		}
	}
	return ArrResult;
}
