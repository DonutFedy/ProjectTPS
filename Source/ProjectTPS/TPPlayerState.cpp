// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPlayerState.h"
#include "TPGameInstance.h"
#include "TPSaveGame.h"

ATPPlayerState::ATPPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 0;
	Exp = 0;
	SaveSlotName = TEXT("Player1");
	CharacterIndex = 0;
}


int32 ATPPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ATPPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

int32 ATPPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 ATPPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}

float ATPPlayerState::GetExpRatio() const
{
// 	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
// 		return 0.0f;
// 
// 	float Result = (float)Exp / (float)CurrentStatData->NextExp;
// 	TPLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
// 	return Result;
	return 0.f;
}

bool ATPPlayerState::AddExp(int32 IncomExp)
{
	return false;
// 	if (CurrentStatData->NextExp == -1)
// 		return false;
// 
// 	bool DidLevelUp = false;
// 	Exp = Exp + IncomExp;
// 	if (Exp >= CurrentStatData->NextExp)
// 	{
// 		Exp -= CurrentStatData->NextExp;
// 		SetCharacterLevel(CharacterLevel + 1);
// 		DidLevelUp = true;
// 	}
// 	OnPlayerStateChanged.Broadcast();
// 	SavePlayerData();
// 	return DidLevelUp;
}

void ATPPlayerState::AddGameScore()
{
	++GameScore;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

void ATPPlayerState::InitPlayerData()
{
	auto TPSaveGame = Cast<UTPSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (TPSaveGame == nullptr)
	{
		TPSaveGame = GetMutableDefault<UTPSaveGame>();
	}

	SetPlayerName(TPSaveGame->PlayerName);
	SetCharacterLevel(TPSaveGame->Level);
	GameScore = 0;
	GameHighScore = TPSaveGame->HighScore;
	Exp = TPSaveGame->Exp;
	CharacterIndex = TPSaveGame->CharacterIndex;
	SavePlayerData();
}

void ATPPlayerState::SavePlayerData()
{
	UTPSaveGame* NewPlayerData = NewObject<UTPSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		TPLOG(Error, TEXT("SaveGame Error!!!"));
	}
}

void ATPPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto TPGameInstance = Cast<UTPGameInstance>(GetGameInstance());
	TPCHECK(TPGameInstance != nullptr);

	CurrentStatData = TPGameInstance->GetTPCharacterData(NewCharacterLevel);
	TPCHECK(CurrentStatData != nullptr);

	CharacterLevel = NewCharacterLevel;
}
