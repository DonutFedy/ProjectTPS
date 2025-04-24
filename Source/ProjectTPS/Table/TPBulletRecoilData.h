// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TPBulletRecoilData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTPBulletRecoilData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> Recoil;
	


};
