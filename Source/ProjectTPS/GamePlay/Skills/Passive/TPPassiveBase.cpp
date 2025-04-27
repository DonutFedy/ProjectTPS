// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPassiveBase.h"
#include "../../../Table/TPSkillTable.h"
#include "../../../Management/TPStageManager.h"

bool UTPPassiveBase::CheckSkillCondition(float DeltaTime, ESkillConditionType ActionType)
{
	// 한 패시브에 여러 효과가 있을 수 있으므로.
	for (int CurPassiveIndex = 0; CurPassiveIndex < CurPassiveInfo.Num(); ++CurPassiveIndex)
	{
		// 효과 적용중이면 무시.
		if (ArrCurSkillConditionInfo[CurPassiveIndex].IsApply)
		{
			if(ArrCurSkillConditionInfo[CurPassiveIndex].CurEffectDuration == -1)
				return false;
			ArrCurSkillConditionInfo[CurPassiveIndex].CurEffectDuration = FMath::Clamp(ArrCurSkillConditionInfo[CurPassiveIndex].CurEffectDuration - DeltaTime, 0.f, CurPassiveInfo[CurPassiveIndex]->EffectDuration);

			if (ArrCurSkillConditionInfo[CurPassiveIndex].CurEffectDuration <= 0.f)
			{
				UnapplyEffect(CurPassiveIndex);
				return true;
			}
			return false;
		}

		// 쿨타임 있다면 계산
		if (ArrCurSkillConditionInfo[CurPassiveIndex].CurRemainCoolTime > 0)
		{
			ArrCurSkillConditionInfo[CurPassiveIndex].CurRemainCoolTime = FMath::Clamp(ArrCurSkillConditionInfo[CurPassiveIndex].CurRemainCoolTime - DeltaTime, 0.f, CurPassiveInfo[CurPassiveIndex]->EffectCooldown);
			return false;
		}
		// 쿨타임이 0에 도달했다면 컨디션 체크.
		switch (CurPassiveInfo[CurPassiveIndex]->ConditionType)
		{
		case ESkillConditionType::SCondition_NONE:
		{
			RunSkill(CurPassiveIndex);
			return true;
		}
		break;
		case ESkillConditionType::SCondition_USE_DODGE:
		case ESkillConditionType::SCondition_CHARACTER_FIRE_GUN:
		case ESkillConditionType::SCondition_ENEMY_HITTED_BULLET:
		{
			// 틱마다 체크하는 종류가 아니라면, 특정 조건에 반응 해야하므로.
			if (ActionType == CurPassiveInfo[CurPassiveIndex]->ConditionType)
			{
				RunSkill(CurPassiveIndex);
				return true;
			}
		}
		break;
		case ESkillConditionType::SCondition_CHARACTER_CURRENT_SHIELD:
		case ESkillConditionType::SCondition_CHARACTER_CURRENT_HP:
		case ESkillConditionType::SCondition_CHARACTER_CURRENT_STAMINA:
		case ESkillConditionType::SCondition_CHARACTER_STAT_SHIELDMAX:
		case ESkillConditionType::SCondition_CHARACTER_STAT_HPMAX:
		case ESkillConditionType::SCondition_CHARACTER_STAT_STAMINAMAX:
		case ESkillConditionType::SCondition_CHARACTER_PASSIVE_NUM:
		case ESkillConditionType::SCondition_CHARACTER_CURRENT_WEAPON_AR:
		case ESkillConditionType::SCondition_CHARACTER_CURRENT_WEAPON_SG:
		case ESkillConditionType::SCondition_CHARACTER_CURRENT_WEAPON_SR:
		case ESkillConditionType::SCondition_CHARACTER_CURRENT_WEAPON_HG:
		{

		}
		break;
		default:
			break;
		}

	}


	return false;
}

void UTPPassiveBase::ReleaseSkill()
{
	for (int CurPassiveIndex = 0; CurPassiveIndex < CurPassiveInfo.Num(); ++CurPassiveIndex)
		UnapplyEffect(CurPassiveIndex);
	CurPassiveInfo.Empty();
	ArrCurSkillConditionInfo.Empty();
}

void UTPPassiveBase::RunSkill(int EffectIndex)
{	
	ArrCurSkillConditionInfo[EffectIndex].CurRemainCoolTime = CurPassiveInfo[EffectIndex]->EffectCooldown;
	ArrCurSkillConditionInfo[EffectIndex].CurEffectDuration = CurPassiveInfo[EffectIndex]->EffectDuration;
}

void UTPPassiveBase::SetupSkill(UTPStageManager* StageMgr)
{
	CurPassiveInfo = StageMgr->GetPassiveSkillInfo(CurSkillInfo.GroupID, CurLV);
	ArrCurSkillConditionInfo.SetNum(CurPassiveInfo.Num());
}

bool UTPPassiveBase::CheckSkillConditionAfterAction(ESkillConditionType ActionType)
{
	return CheckSkillCondition(0.f, ActionType);
}

void UTPPassiveBase::UnapplyEffect(int TargetIndex)
{
	ArrCurSkillConditionInfo[TargetIndex].IsApply = false;
	CurOwnerStatComp->RemoveEffect(SkillSerializeIndex, CurPassiveInfo[TargetIndex]->EffectType);
}
