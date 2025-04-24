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
	ESkillEffectType EffectType; // ȿ�� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectValue; // ȿ�� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillTargetType TargetType; // Ÿ�� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetValue; // Ÿ�� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillAreaType AreaType; // ��/�׸�/���� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectDuration; // ȿ�� ���ӽð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TickTime; // �ݺ��ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectCooldown; // ���� ��Ÿ��
};
