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
	float CurRemainCoolTime;// �ش� �ð��� 0�̵Ǹ� ���� üũ
	float CurEffectDuration; // ȿ�� ���ӽð�
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
	// ��ų �ߵ�
	virtual void RunSkill(int EffectIndex)  override;
	// ���� ��ų ����
	virtual void SetupSkill(class UTPStageManager* StageMgr) override;
	// Ư�� �׼��� �Ҹ��� �Լ�.
	virtual bool CheckSkillConditionAfterAction(ESkillConditionType ActionType) override; 

	virtual void UnapplyEffect(int TargetIndex);

	TArray<FTPPassiveGroupTable*> CurPassiveInfo;


	TArray<FTPSkillConditionInfo> ArrCurSkillConditionInfo; 
	
};
