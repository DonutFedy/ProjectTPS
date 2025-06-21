// Fill out your copyright notice in the Description page of Project Settings.


#include "TPActive_AttackBuf.h"
#include "../Buf/TPBufBase.h"
#include "../../../TPCharacter.h"
#include "../TPSkillBase.h"
#include "../TPSkillController.h"
#include "Obj/TPGranade.h"
#include "GameFramework/Actor.h"



void UTPActive_AttackBuf::RunSkill(int EffectIndex)
{

	Super::RunSkill(EffectIndex);

	TPCHECK(CurOwnerStatComp != nullptr);
	FTPBufInfo newBufInfo;
	newBufInfo.EffectDuration = CurActiveInfo[EffectIndex]->EffectDuration;
	newBufInfo.EffectValue = CurActiveInfo[EffectIndex]->EffectValue;
	newBufInfo.EffectType = CurActiveInfo[EffectIndex]->EffectType;
	ArrCurSkillConditionInfo[EffectIndex].IsApply = CurOwnerStatComp->AddBuf(SkillSerializeIndex, newBufInfo);
}
