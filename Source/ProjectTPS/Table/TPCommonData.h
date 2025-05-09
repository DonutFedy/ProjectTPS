// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TPCommonData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTPSetupSkillData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SkillIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Level;
};
