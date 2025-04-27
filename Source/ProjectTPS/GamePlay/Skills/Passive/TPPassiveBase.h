// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../ProjectTPS.h"
#include "../TPSkillBase.h"
#include "UObject/NoExportTypes.h"
#include "TPPassiveBase.generated.h"

USTRUCT(BlueprintType)
struct FTPSkillConditionInfo
{
	GENERATED_BODY()
public:
	FTPSkillConditionInfo():CurRemainCoolTime(0.f), CurEffectDuration(0.f), IsApply(false)
	{
	}
	float CurRemainCoolTime;// 해당 시간이 0이되면 조건 체크
	float CurEffectDuration; // 효과 지속시간
	bool IsApply;
};

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPPassiveBase : public UTPSkillBase
{
	GENERATED_BODY()
public:
	virtual bool CheckSkillCondition(float DeltaTime, ESkillConditionType ActionType = ESkillConditionType::SCondition_NONE) override;

	virtual void ReleaseSkill() override;
protected:
	// 스킬 발동
	virtual void RunSkill(int EffectIndex)  override;
	// 실제 스킬 세팅
	virtual void SetupSkill(class UTPStageManager* StageMgr) override;
	// 특정 액션후 불리는 함수.
	virtual bool CheckSkillConditionAfterAction(ESkillConditionType ActionType) override; 

	virtual void UnapplyEffect(int TargetIndex);

	TArray<FTPPassiveGroupTable*> CurPassiveInfo;


	TArray<FTPSkillConditionInfo> ArrCurSkillConditionInfo; 
	
};
