// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillController.h"
#include "../../Management/TPStageManager.h"
#include "TPSkillBase.h"
#include "../../Table/TPSkillTable.h"
#include "../../TPSkillComponent.h"
#include "Passive/TPPassiveBase.h"

void UTPSkillController::InitSkill(FTPSkillInitData& InitInfo, UTPSkillComponent* SkillComp, UTPStageManager* StageMgr)
{
	FTPSkillTable* CurSkillInfo = StageMgr->GetSkillInfo(InitInfo.SkillIndex);
	TPCHECK(CurSkillInfo != nullptr);

	switch (CurSkillInfo->SkillType)
	{
		case ESkillType::ST_PASSIVE:
		{
			// 원래는 뭔지 구분해야됨.
			CurSkillObj = NewObject<UTPPassiveBase>();
		}
			break;
		default:
			break;
	}
	TPCHECK(CurSkillObj != nullptr);
	CurSkillObj->InitSkill(InitInfo.SkillLv, *CurSkillInfo, SkillComp, StageMgr);
}

bool UTPSkillController::CheckSkillCondition()
{
	TPCHECK(CurSkillObj != nullptr, false);

	return CurSkillObj->CheckSkillCondition();
}
