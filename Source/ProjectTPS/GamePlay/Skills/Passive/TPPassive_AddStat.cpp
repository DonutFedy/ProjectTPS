// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPassive_AddStat.h"
#include "../Buf/TPBufBase.h"



void UTPPassive_AddStat::RunSkill(int EffectIndex)
{
	Super::RunSkill(EffectIndex);

	TPCHECK(CurOwnerStatComp != nullptr);

	ArrCurSkillConditionInfo[EffectIndex].IsApply = CurOwnerStatComp->AddBuf(SkillSerializeIndex, *CurPassiveInfo[EffectIndex]);
}
