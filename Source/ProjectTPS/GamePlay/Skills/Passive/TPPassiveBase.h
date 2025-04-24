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
	// ��ų �ߵ�
	virtual void RunSkill()  override;
	// ���� ��ų ����
	virtual void SetupSkill(class UTPStageManager* StageMgr) override;

	FTPPassiveGroupTable CurPassiveInfo;
};
