// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TPChapterTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTTPS_API FTPChapterTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Chapter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxStage;
};
