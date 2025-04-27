// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillBase.h"
#include "../../Management/TPStageManager.h"

int UTPSkillBase::SkillGenerateIndex = 0;

void UTPSkillBase::InitSkill(int Lv, FTPSkillTable& CurSkillTableInfo, TObjectPtr<UTPSkillComponent> SkillComp, UTPStageManager* StageMgr)
{
	SkillSerializeIndex = ++SkillGenerateIndex;
	CurOwnerStatComp = SkillComp;
	CurLV = Lv;
	CurSkillInfo = CurSkillTableInfo;
	SetupSkill(StageMgr);
}
