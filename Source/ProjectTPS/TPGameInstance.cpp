// Fill out your copyright notice in the Description page of Project Settings.


#include "TPGameInstance.h"
#include "Table/TPCharacterData.h"
#include "Table/TPEnemyData.h"
#include "Table/TPWeaponTable.h"
#include "Table/TPBulletRecoilData.h"
#include "Runtime/Engine/Classes/Engine/LevelStreamingDynamic.h"
#include "TPCharacter.h"
#include "TPPlayerController.h"
#include "TPPlayerState.h"
#include "Management/TPStageManager.h"
#include "GamePlay/Skills/TPSkillBase_Legacy.h"
#include "GamePlay/Skills/TPSkillBase.h"
#include "TPSkillComponent.h"


UTPGameInstance::UTPGameInstance()
{

	FString WeaponDataPath = TEXT("/Script/Engine.DataTable'/Game/Table/InGame/WeaponTable.WeaponTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TPWEAPON(*WeaponDataPath);
	TPCHECK(DT_TPWEAPON.Succeeded());
	TPWeaponTable = DT_TPWEAPON.Object;
	TPCHECK(TPWeaponTable->GetRowMap().Num() > 0);

	FString RecoilDataPath = TEXT("/Script/Engine.DataTable'/Game/Table/InGame/BulletRecoilTable.BulletRecoilTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TPRECOIL(*RecoilDataPath);
	TPCHECK(DT_TPRECOIL.Succeeded());
	TPRecoilTable = DT_TPRECOIL.Object;
	TPCHECK(TPRecoilTable->GetRowMap().Num() > 0);


	BulletIndex = 0;
}


void UTPGameInstance::Init()
{
	Super::Init();

	InitManager();
	BulletIndex = 0;
}

FTPCharacterData* UTPGameInstance::GetTPCharacterData(int32 InLevel)
{
	return StageManager->GetTPCharacterData(InLevel);
}

FTPEnemyData* UTPGameInstance::GetTPEnemyData(int32 InIndex)
{
	return StageManager->GetTPEnemyData(InIndex);
}

// FTPSkillInfo UTPGameInstance::GetSkillInfo(FTPSkillInitData& InitData)
// {
// 	return StageManager->GetSkillInfo(InitData);
// }

FTPWeaponTable* UTPGameInstance::GetTPWeaponData(int32 InIndex)
{
	return TPWeaponTable->FindRow<FTPWeaponTable>(*FString::FromInt(InIndex), TEXT(""));
}


FTPBulletRecoilData* UTPGameInstance::GetTPRecilData(int32 InIndex)
{
	return TPRecoilTable->FindRow<FTPBulletRecoilData>(*FString::FromInt(InIndex), TEXT(""));
}

UTPSkillBase_Legacy* UTPGameInstance::GetSkillInfo_Legacy(FTPSkillInitData& InitData)
{
	return StageManager->GetSkillInfo_Legacy(InitData);
}

UTPSkillController* UTPGameInstance::GetSkillController(FTPSkillInitData& InitData, UTPSkillComponent* SkillComp)
{
	return StageManager->GetSkillController(InitData, SkillComp);
}

int UTPGameInstance::GetTPRecilDataNum()
{
	return TPRecoilTable->GetRowMap().Num();
}

TArray<class ATPCharacter*> UTPGameInstance::GetMainCharacter()
{
	return StageManager->GetMainCharacter();
}

TArray<class ATPCharacter*> UTPGameInstance::GetEnemies()
{
	return StageManager->GetEnemies();
}

void UTPGameInstance::StartGame()
{
	TPCHECK(StageManager);
	if (StageManager)
		StageManager->StartGame();
}

void UTPGameInstance::NextStage()
{
	TPCHECK(StageManager);
	if (StageManager)
		StageManager->SetManagerStep(UTPStageManager::EStageManagerStep::SMS_LOAD_STAGE);
}

void UTPGameInstance::InitManager()
{
	StageManager = NewObject<UTPStageManager>();
	TPCHECK(StageManager);
	if(StageManager)
		StageManager->InitManager(this);
}