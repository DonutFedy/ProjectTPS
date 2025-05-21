// Fill out your copyright notice in the Description page of Project Settings.


#include "TPActiveBase.h"
#include "../../../Table/TPSkillTable.h"
#include "../../../Management/TPStageManager.h"
#include "../../../ProjectTPS.h"

bool UTPActiveBase::CheckSkillCondition(float DeltaTime, ESkillConditionType ActionType)
{
	for (int CurActiveIndex = 0; CurActiveIndex < CurActiveInfo.Num(); ++CurActiveIndex)
	{
		// 효과 적용중이면 무시.
		if (ArrCurSkillConditionInfo[CurActiveIndex].IsApply)
		{
			if(ArrCurSkillConditionInfo[CurActiveIndex].CurEffectDuration == -1)
				return false;
			ArrCurSkillConditionInfo[CurActiveIndex].CurEffectDuration = FMath::Clamp(ArrCurSkillConditionInfo[CurActiveIndex].CurEffectDuration - DeltaTime, 0.f, CurActiveInfo[CurActiveIndex]->EffectDuration);

			if (ArrCurSkillConditionInfo[CurActiveIndex].CurEffectDuration <= 0.f)
			{
				UnapplyEffect(CurActiveIndex);
				return true;
			}
			return false;
		}

		// 쿨타임 있다면 계산
		if (ArrCurSkillConditionInfo[CurActiveIndex].CurRemainCoolTime > 0)
		{
			ArrCurSkillConditionInfo[CurActiveIndex].CurRemainCoolTime = FMath::Clamp(ArrCurSkillConditionInfo[CurActiveIndex].CurRemainCoolTime - DeltaTime, 0.f, CurActiveInfo[CurActiveIndex]->UseCooldown);
			return false;
		}

	}


	return false;
}

void UTPActiveBase::ReleaseSkill()
{
	for (int CurActiveIndex = 0; CurActiveIndex < CurActiveInfo.Num(); ++CurActiveIndex)
		UnapplyEffect(CurActiveIndex);
	CurActiveInfo.Empty();
	ArrCurSkillConditionInfo.Empty();
}

int UTPActiveBase::GetEffectValue(ESkillEffectType InEffectType)
{
	for (auto CurActiveSkillInfo : CurActiveInfo)
	{
		if(CurActiveSkillInfo->EffectType == InEffectType)
			return CurActiveSkillInfo->EffectValue;
	}
	return INDEX_NONE;
}

bool UTPActiveBase::ReleaseActiveSkill()
{
	IsReadyToUse = false;
	return true;
}

void UTPActiveBase::RunSkill(int EffectIndex)
{	
	ArrCurSkillConditionInfo[EffectIndex].CurRemainCoolTime = CurActiveInfo[EffectIndex]->UseCooldown;
	ArrCurSkillConditionInfo[EffectIndex].CurEffectDuration = CurActiveInfo[EffectIndex]->EffectDuration;
}

void UTPActiveBase::SetupSkill(UTPStageManager* StageMgr)
{
	CurActiveInfo = StageMgr->GetActiveSkillInfo(CurSkillInfo.GroupID, CurLV);
	ArrCurSkillConditionInfo.SetNum(CurActiveInfo.Num());
}

bool UTPActiveBase::CheckSkillConditionAfterAction(ESkillConditionType ActionType)
{
	for (int CurActiveIndex = 0; CurActiveIndex < CurActiveInfo.Num(); ++CurActiveIndex)
	{
		// 효과 적용중이면 무시.
		if (ArrCurSkillConditionInfo[CurActiveIndex].IsApply)
		{
			return false;
		}
		// 쿨타임 있다면 계산
		if (ArrCurSkillConditionInfo[CurActiveIndex].CurRemainCoolTime > 0)
		{
			return false;
		}
		// 쿨타임이 0에 도달했다면 컨디션 체크.
		switch (CurActiveInfo[CurActiveIndex]->ConditionType)
		{
		case ESkillConditionType::SCondition_CHARACTER_USE_SKILL:
		{
			RunSkill(CurActiveIndex);
			return true;
		}
		default:
			break;
		}
	}

	return false;
}

void UTPActiveBase::UnapplyEffect(int TargetIndex)
{
	ArrCurSkillConditionInfo[TargetIndex].IsApply = false;
	CurOwnerStatComp->RemoveEffect(SkillSerializeIndex, CurActiveInfo[TargetIndex]->EffectType);
}

void UTPActiveBase::ReleaseObj(TObjectPtr<AActor> InReleaseObj)
{
	FTPActiveSkillObjState* FoundSkillObj = ArrSkillObj.FindByPredicate(
		[InReleaseObj](const FTPActiveSkillObjState& a) { return a.RealObj == InReleaseObj; }
	);
	if (FoundSkillObj == nullptr)
	{
		InReleaseObj->Destroy();
		InReleaseObj = nullptr;
		return;
	}
	FoundSkillObj->IsUsing = false;
}
