// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TPCommonData.h"
#include "TPCharacterData.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FTPCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ShieldMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShieldRest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShieldRecover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HpMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HpRest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HpRecover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StaminaMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaRest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaRecover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pierce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MoveSpd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadSpd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int JumpForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTPSetupSkillData> SetupSkills;
};
