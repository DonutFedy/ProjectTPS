// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectTPS.h"
#include "Engine/DataTable.h"
#include "TPSkillTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTTPS_API FTPSkillTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillCategoryType SkillCategoryType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillUseType SkillUseType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxLV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GroupID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillBP_Path;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTTPS_API FTPPassiveGroupTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GroupID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Lv;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillConditionType ConditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConditionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillEffectType EffectType; // 효과 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectValue; // 효과 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTargetType TargetType; // 타겟 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetValue; // 타겟 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillAreaType AreaType; // 원/네모/직선 등
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectDuration; // 효과 지속시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TickTime; // 반복시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectCooldown; // 재사용 쿨타임
};
