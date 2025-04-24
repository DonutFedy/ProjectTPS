// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TPSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UTPSaveGame();

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Exp;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 HighScore;

	UPROPERTY()
	int32 CharacterIndex;
};
