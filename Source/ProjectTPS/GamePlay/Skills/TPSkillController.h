// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../Management/TPStageManager.h"
#include "TPSkillController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPSkillController : public UObject
{
	GENERATED_BODY()
public:
	void InitSkill(FTPSkillInitData& InitInfo, class UTPSkillComponent* SkillComp, class UTPStageManager* StageMgr);
	void ReleaseSkill(); // 스킬 제거하기위함.


	// 스킬의 발동 여부 체크.
	void CheckSkillCondition(float DeltaTime);
	void CheckSkillConditionAfterAction(ESkillConditionType ActionType); // 특정 액션후 불리는 함수.
	int GetSkillIndex(){ return CurSetupInfo.SkillIndex;}

	FString _GetSkillLog();
protected:
	TArray<TObjectPtr<class UTPSkillBase>> GetPassiveObj(int InSkillLV, class UTPStageManager* StageMgr, struct FTPSkillTable* Info);

public:
protected:
	FTPSkillInitData CurSetupInfo;

	UPROPERTY()
	TArray<TObjectPtr<class UTPSkillBase>> ArrCurSkillObj;
};
