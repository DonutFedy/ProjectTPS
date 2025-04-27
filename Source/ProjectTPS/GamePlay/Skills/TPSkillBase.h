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
	static int SkillGenerateIndex; // 스킬 InitSkill 할때 값이 증가함.
	// 스킬 세팅.
	void InitSkill(int Lv, FTPSkillTable& CurSkillTableInfo, TObjectPtr<UTPSkillComponent> SkillComp, class UTPStageManager* StageMgr);
	virtual void ReleaseSkill(){}
protected:
	// 실제 스킬 세팅
	virtual void SetupSkill(class  UTPStageManager* StageMgr){}
public:
	// 스킬의 발동 여부 체크.
	virtual bool CheckSkillCondition(float DeltaTime, ESkillConditionType ActionType = ESkillConditionType::SCondition_NONE){ return false; }
	virtual bool CheckSkillConditionAfterAction(ESkillConditionType ActionType){return false;} // 특정 액션후 불리는 함수.

protected:
	// 스킬 기본 정보
	UPROPERTY()
	FTPSkillTable CurSkillInfo;
	UPROPERTY()
	TObjectPtr<UTPSkillComponent> CurOwnerStatComp; // 버프나 각종 작용을 위함.
	int CurLV;
	
	// 스킬 발동. CheckSkillCondition 에서 호출해준다.
	virtual void RunSkill(int EffectIndex){}
	

	int SkillSerializeIndex; // 스킬 할당될때마다의 고유 식별자.
};
