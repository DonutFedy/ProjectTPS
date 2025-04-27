// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPassiveBase.h"
#include "../../../Table/TPSkillTable.h"
#include "../../../Management/TPStageManager.h"

bool UTPPassiveBase::CheckSkillCondition(float DeltaTime, ESkillConditionType ActionType)
{
	// �� �нú꿡 ���� ȿ���� ���� �� �����Ƿ�.
	for (int CurPassiveIndex = 0; CurPassiveIndex < CurPassiveInfo.Num(); ++CurPassiveIndex)
	{
		// ȿ�� �������̸� ����.
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

		// ��Ÿ�� �ִٸ� ���
		if (ArrCurSkillConditionInfo[CurPassiveIndex].CurRemainCoolTime > 0)
		{
			ArrCurSkillConditionInfo[CurPassiveIndex].CurRemainCoolTime = FMath::Clamp(ArrCurSkillConditionInfo[CurPassiveIndex].CurRemainCoolTime - DeltaTime, 0.f, CurPassiveInfo[CurPassiveIndex]->EffectCooldown);
			return false;
		}
		// ��Ÿ���� 0�� �����ߴٸ� ����� üũ.
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
			// ƽ���� üũ�ϴ� ������ �ƴ϶��, Ư�� ���ǿ� ���� �ؾ��ϹǷ�.
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
