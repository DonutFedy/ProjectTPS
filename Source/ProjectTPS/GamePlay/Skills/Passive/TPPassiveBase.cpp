// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPassiveBase.h"
#include "../../../Table/TPSkillTable.h"
#include "../../../Management/TPStageManager.h"

bool UTPPassiveBase::CheckSkillCondition()
{
	return false;
}

void UTPPassiveBase::RunSkill()
{

}

void UTPPassiveBase::SetupSkill(UTPStageManager* StageMgr)
{
	CurPassiveInfo = *StageMgr->GetPassiveSkillInfo(CurSkillInfo.GroupID, CurLV);


}
