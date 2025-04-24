// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TPSkillController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPSkillController : public UObject
{
	GENERATED_BODY()
public:
	void InitSkill(struct FTPSkillInitData& InitInfo, class UTPSkillComponent* SkillComp, class UTPStageManager* StageMgr);

	// ��ų�� �ߵ� ���� üũ.
	bool CheckSkillCondition();


public:
protected:
	UPROPERTY()
	class UTPSkillBase* CurSkillObj;
};
