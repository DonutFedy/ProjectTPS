// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TPSkillBase_Legacy.h"
#include "TPSkillPassive_Bullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPSkillPassive_Bullet : public UTPSkillBase_Legacy
{
	GENERATED_BODY()
	
	/*
	/ 탄 관련 /
	탄 분리					발사되는 총알 개수 증가
	탄 튕김					"적에게 맞춘 총알이 확률적으로 일정 거리 내의 적을 향해 도탄"
	밀어내기 추가				"적에게 공격 타격 시 일정확률로 적 밀어내기"
	*/
};
