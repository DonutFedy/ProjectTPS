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
	void ReleaseSkill(); // ��ų �����ϱ�����.


	// ��ų�� �ߵ� ���� üũ.
	void CheckSkillCondition(float DeltaTime);
	void CheckSkillConditionAfterAction(ESkillConditionType ActionType); // Ư�� �׼��� �Ҹ��� �Լ�.
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
