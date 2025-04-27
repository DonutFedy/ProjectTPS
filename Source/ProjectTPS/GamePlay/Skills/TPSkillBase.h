// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../ProjectTPS.h"
#include "UObject/NoExportTypes.h"
#include "../../Table/TPSkillTable.h"
#include "TPSkillController.h"
#include "../../TPSkillComponent.h"
#include "TPSkillBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPSkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	static int SkillGenerateIndex; // ��ų InitSkill �Ҷ� ���� ������.
	// ��ų ����.
	void InitSkill(int Lv, FTPSkillTable& CurSkillTableInfo, TObjectPtr<UTPSkillComponent> SkillComp, class UTPStageManager* StageMgr);
	virtual void ReleaseSkill(){}
protected:
	// ���� ��ų ����
	virtual void SetupSkill(class  UTPStageManager* StageMgr){}
public:
	// ��ų�� �ߵ� ���� üũ.
	virtual bool CheckSkillCondition(float DeltaTime, ESkillConditionType ActionType = ESkillConditionType::SCondition_NONE){ return false; }
	virtual bool CheckSkillConditionAfterAction(ESkillConditionType ActionType){return false;} // Ư�� �׼��� �Ҹ��� �Լ�.

protected:
	// ��ų �⺻ ����
	UPROPERTY()
	FTPSkillTable CurSkillInfo;
	UPROPERTY()
	TObjectPtr<UTPSkillComponent> CurOwnerStatComp; // ������ ���� �ۿ��� ����.
	int CurLV;
	
	// ��ų �ߵ�. CheckSkillCondition ���� ȣ�����ش�.
	virtual void RunSkill(int EffectIndex){}
	

	int SkillSerializeIndex; // ��ų �Ҵ�ɶ������� ���� �ĺ���.
};
