// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectTPS.h"
#include "UObject/NoExportTypes.h"
#include "TPEnemySpawner.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API ATPEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnerType CurSpawnerType;

	ATPEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SpawnIndex;
	float GetSpawnRange(){return SpawnRange;}
	int GetSpawnIndex(){return SpawnIndex;}
	FVector GetSpawnPoint();
	void OnConstruction(const FTransform& Transform);

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UStaticMeshComponent* DebugMesh;
#endif
};
