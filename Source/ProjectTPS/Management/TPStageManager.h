// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectTPS.h"
#include "UObject/NoExportTypes.h"
#include "TPStageManager.generated.h"

USTRUCT(BlueprintType)
struct FTPSkillInitData
{
	GENERATED_BODY()
public:
	FTPSkillInitData():SkillIndex(0), SkillLv(0), SkillType(ESkillType::ST_NONE)
	{}

	int SkillIndex;
	int SkillLv;
	ESkillType SkillType;
};
// 
// USTRUCT(BlueprintType)
// struct FTPSkillInfo
// {
// 	GENERATED_BODY()
// public:
// 	FTPSkillInfo() :SkillIndex(0), SkillLv(0), ColdTime(0.f), CurrentColdTime(0.f), SkillType(ESkillType::NONE), BufType(EBufType::BT_NONE), BufStatType(EBufStatType::BST_NONE), BufValue(0.f)
// 	{
// 	}
// 
// 	int SkillIndex;
// 	int SkillLv;
// 	float ColdTime;
// 	float CurrentColdTime;
// 	ESkillType SkillType;
// 	EBufType BufType;
// 	EBufStatType BufStatType;
// 	float BufValue;
// };


/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPStageManager : public UObject
{
	GENERATED_BODY()
	

public:
	class UTPGameInstance* GameInstance;

	enum class EStageManagerStep
	{
		SMS_NONE,
		SMS_INIT,
		SMS_START_GAME,
		SMS_INIT_CHAPTER,
		SMS_LOAD_STAGE,
		SMS_PLAY_STAGE,
		SMS_END_STAGE,
		SMS_END_CHAPTER,
		SMS_END_GAME,
	};


	UTPStageManager();
	void InitManager(class UTPGameInstance* InGameInstance);

	void StartGame(); // 게임을 시작.


	class APlayerStart* GetStartPoint();

#pragma region Skill Data
private:

public:
	//FTPSkillInfo GetSkillInfo(FTPSkillInitData& InitData);
	class UTPSkillBase_Legacy* GetSkillInfo_Legacy(FTPSkillInitData& InitData);
	class  UTPSkillController* GetSkillController(FTPSkillInitData& InitData, class UTPSkillComponent* SkillComp);
	struct FTPSkillTable* GetSkillInfo(int InSkillIndex);
	TArray<struct FTPPassiveGroupTable*> GetPassiveSkillInfo(int InGroupID, int InLv);
	ESkillEffectType GetEffectType(ESkillType InSkillType, int InGroupID, int InLv);
private:
	
#pragma endregion



#pragma region Chapter & Stage
public:
	void SetManagerStep(EStageManagerStep NewStep);
protected:
	FString GetStageName();
	void SetNextChapter();
	void SetNextStage();
	void OpenGate();
protected:
	void PrevStageUnload();
public:
	UFUNCTION()
	void OnCompletedUnloadStage();
protected:
	void StageLoad(); // 다음 스테이지를 로드.
public:
	UFUNCTION()
	void OnCompletedLoadStage();
protected:
	void CharacterLoad();
	void NextWave(); // Enemy Load & Set
	void PlayStage();
	void GameEnd();


private:
	int CurrentChapterIndex;
	int ChapterIndexMax;
	int CurrentStageNum;
	int CurrentStageIndex;
	int StageIndexMax;
	int CurrentWaveIndex;
	int WaveIndexMax;
	

	UPROPERTY()
	class ATPStageGate* StageGate;


	EStageManagerStep CurrentStep;
	UPROPERTY()
	UDataTable*		TPChapterData;
	UPROPERTY()
	UDataTable*		TPStageData;
	FString			CurrentStageName;
	UPROPERTY()
	TArray<struct FTPStageTable> CurChapterStages;
	UPROPERTY()
	TMap<int, class ATPEnemySpawner*> CurStageSpawner;
	UPROPERTY()
	class ATPEnemySpawner* CurStageGateSpawner;

#pragma endregion


#pragma region Chapter/Stage/Wave End Check
	void OnCharacterDead();
	bool CheckWaveEnd();
	bool CheckStageEnd();
	bool CheckChapterEnd();
#pragma endregion

#pragma region Skill
	UPROPERTY()
	UDataTable* TPPassiveTable;
	
	UPROPERTY()
	UDataTable* TPPassiveGroupTable;
	UPROPERTY()
	UDataTable* TPSkillTable;
#pragma endregion



#pragma region Player
private:

	UPROPERTY()
	UDataTable* TPCharacterTable;

public:
	struct FTPCharacterData* GetTPCharacterData(int32 InLevel);
	TArray<class ATPCharacter*> GetMainCharacter();
	bool IsExistMainCharacter();

#pragma endregion


#pragma region Enemy
private:
	UPROPERTY()
	TArray<class ATPCharacter*> ArrPlayers;
	UPROPERTY()
	TArray<class ATPCharacter*> ArrEnemies;
	
	UPROPERTY()
	UDataTable* TPEnemyTable;
public:
	struct FTPEnemyData* GetTPEnemyData(int32 InIndex);
	TArray<class ATPCharacter*> GetEnemies();
#pragma endregion

};
