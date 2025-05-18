// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPPassiveBase.h"
#include "TPPassive_Bullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPPassive_Bullet : public UTPPassiveBase
{
	GENERATED_BODY()
public:
	// 스킬 발동
	virtual void RunSkill(int EffectIndex)  override;
};
