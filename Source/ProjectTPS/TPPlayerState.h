// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);
/**
 * 
 */
UCLASS()
class PROJECTTPS_API ATPPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATPPlayerState();


	int32 GetGameScore() const;
	int32 GetGameHighScore() const;
	int32 GetCharacterLevel() const;
	int32 GetCharacterIndex() const;
	FString SaveSlotName;
	float GetExpRatio() const;
	bool AddExp(int32 IncomExp);
	void AddGameScore();

	void InitPlayerData();
	void SavePlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
	int32 GameScore;
	
	UPROPERTY()
	int32 GameHighScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	int32 Exp;



private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FTPCharacterData* CurrentStatData;
	UPROPERTY(Transient)
	int32 CharacterIndex;
};
