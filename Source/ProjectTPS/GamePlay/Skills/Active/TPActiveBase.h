// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../ProjectTPS.h"
#include "../TPSkillBase.h"
#include "UObject/NoExportTypes.h"
#include "../../../Table/TPSkillTable.h"
#include "TPActiveBase.generated.h"

USTRUCT(BlueprintType)
struct FTPActiveSkillObjState
{
	GENERATED_BODY()
public:
	FTPActiveSkillObjState():IsUsing(false), RealObj(nullptr)
	{}
	bool IsUsing;
	TObjectPtr<AActor> RealObj;
};


/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPActiveBase : public UTPSkillBase
{
	GENERATED_BODY()
public:
	virtual bool CheckSkillCondition(float DeltaTime, ESkillConditionType ActionType = ESkillConditionType::SCondition_NONE) override;

	// 특정 액션후 불리는 함수.
	virtual bool CheckSkillConditionAfterAction(ESkillConditionType ActionType) override;
	virtual void ReleaseSkill() override;
	virtual int GetEffectValue(ESkillEffectType InEffectType) override;
	bool GetIsReadyToUse(){ return IsReadyToUse; }
	virtual bool ReleaseActiveSkill();
protected:
	// 스킬 발동
	virtual void RunSkill(int EffectIndex)  override;
	// 실제 스킬 세팅
	virtual void SetupSkill(class UTPStageManager* StageMgr) override;

	virtual void UnapplyEffect(int TargetIndex);
public:
	virtual void ReleaseObj(TObjectPtr<AActor> InReleaseObj);
protected:

	TArray<FTPActiveGroupTable*> CurActiveInfo;
	
	TArray<FTPActiveSkillObjState> ArrSkillObj; // 사용되는 스킬...

	bool IsReadyToUse; // 스킬 사용 전인지 체크.
};
