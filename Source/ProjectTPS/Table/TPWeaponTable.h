// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TPWeaponTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTTPS_API FTPWeaponTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdsTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HipAccuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HipAccuracyDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdsAccuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdsAccuracyDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AccuracyRecover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinAccuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Magazine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StartAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GetAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChangeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Pierce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FireVFX_Path;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FireSFX_Path;
};
