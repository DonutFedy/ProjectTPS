// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TPStageTable.generated.h"
/**
 * 
 */
 USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnemyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Count;
};


USTRUCT(BlueprintType)
struct PROJECTTPS_API FTPStageTable : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Chapter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Stage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StageName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Wave;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> SpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemySpawnInfo> SpawnEnemies;
};
