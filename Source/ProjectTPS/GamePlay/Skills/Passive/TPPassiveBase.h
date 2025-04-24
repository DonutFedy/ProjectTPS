// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../ProjectTPS.h"
#include "../TPSkillBase.h"
#include "UObject/NoExportTypes.h"
#include "TPPassiveBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPPassiveBase : public UTPSkillBase
{
	GENERATED_BODY()
public:
	virtual bool CheckSkillCondition() override;

protected:
	// 스킬 발동
	virtual void RunSkill()  override;
	// 실제 스킬 세팅
	virtual void SetupSkill(class UTPStageManager* StageMgr) override;

	FTPPassiveGroupTable CurPassiveInfo;
};
