// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../ProjectTPS.h"
#include "TPActiveBase.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "TPActive_HealZone.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPActive_HealZone : public UTPActiveBase
{
	GENERATED_BODY()
public:
	// 스킬 발동
	virtual void RunSkill(int EffectIndex)  override;


};
