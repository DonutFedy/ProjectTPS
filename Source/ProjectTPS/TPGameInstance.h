// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "Management/TPStageManager.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "TPGameInstance.generated.h"

// USTRUCT(BlueprintType)
// struct FTPCharacterData : public FTableRowBase
// {
// 	GENERATED_BODY()
// 
// public:
// 	FTPCharacterData() : Level(1), MaxHP(100.f), Attack(10.f), DropExp(10), NextExp(30) {}
// 
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	int32 Level;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	float MaxHP;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	float Attack;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	int32 DropExp;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
// 	int32 NextExp;
// };

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTPGameInstance();
	virtual void Init() override;
	struct FTPCharacterData* GetTPCharacterData(int32 InLevel);
	struct FTPEnemyData* GetTPEnemyData(int32 InIndex);
	struct FTPWeaponTable* GetTPWeaponData(int32 InIndex);
	struct FTPBulletRecoilData* GetTPRecilData(int32 InIndex);
	//struct FTPSkillInfo GetSkillInfo(FTPSkillInitData& InitData);
	class UTPSkillBase_Legacy* GetSkillInfo_Legacy(FTPSkillInitData& InitData);
	class UTPSkillController* GetSkillController(FTPSkillInitData& InitData, class UTPSkillComponent* SkillComp);
	int GetTPRecilDataNum();


	TArray<class ATPCharacter*> GetMainCharacter();
	TArray<class ATPCharacter*> GetEnemies();

	FStreamableManager StreamableManager;


	void StartGame();
	void NextStage();

private:

//====================Manager====================
protected:
	void	InitManager();
	UPROPERTY()
	class UTPStageManager* StageManager;
private:
	UPROPERTY()
	UDataTable* TPWeaponTable;
	UPROPERTY()
	UDataTable* TPRecoilTable;


	UPROPERTY(VisibleAnywhere)
	int BulletIndex;
public:
	int GetNextBulletIndex() { return ++BulletIndex; }
};
