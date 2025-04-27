// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPPassiveBase.h"
#include "TPPassive_AddStat.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPPassive_AddStat : public UTPPassiveBase
{
	GENERATED_BODY()
public:
	// ��ų �ߵ�
	virtual void RunSkill(int EffectIndex)  override;
};
