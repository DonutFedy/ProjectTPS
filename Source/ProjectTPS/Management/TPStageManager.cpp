// Fill out your copyright notice in the Description page of Project Settings.


#include "TPStageManager.h"
#include "../TPCharacter.h"
#include "../TPPlayerState.h"
#include "../Table/TPStageTable.h"
#include "../Table/TPChapterTable.h"
#include "../Table/TPEnemyData.h"
#include "../Table/TPCharacterData.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerStart.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "../GamePlay/TPEnemySpawner.h"
#include "../TPGameInstance.h"
#include "../GamePlay/TPStageGate.h"
#include "../Table/TPPassiveSkillTable.h"
#include "../GamePlay/Skills/TPSkillBase_Legacy.h"
#include "../GamePlay/Skills/TPSkillBase.h"
#include "../GamePlay/Skills/TPSkillController.h"
#include "../TPSkillComponent.h"



UTPStageManager::UTPStageManager()
{
	// �������� ���̺� �ε�
	FString ChapterDataPath = TEXT("/Script/Engine.DataTable'/Game/Table/InGame/ChapterTable.ChapterTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHAPTER(*ChapterDataPath);
	TPCHECK(DT_CHAPTER.Succeeded());
	TPChapterData = DT_CHAPTER.Object;
	TPCHECK(TPChapterData->GetRowMap().Num() > 0);

	FString StageDataPath = TEXT("/Script/Engine.DataTable'/Game/Table/InGame/StageTable.StageTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_STAGE(*StageDataPath);
	TPCHECK(DT_STAGE.Succeeded());
	TPStageData = DT_STAGE.Object;
	TPCHECK(TPStageData->GetRowMap().Num() > 0);
	// ĳ���� ���̺� �ε�
	FString CharacterDataPath = TEXT("/Script/Engine.DataTable'/Game/Table/InGame/CharacterTable.CharacterTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TPCHARACTER(*CharacterDataPath);
	TPCHECK(DT_TPCHARACTER.Succeeded());
	TPCharacterTable = DT_TPCHARACTER.Object;
	TPCHECK(TPCharacterTable->GetRowMap().Num() > 0);
	// �ֳʹ� ���̺� �ε�
	FString EnemyDataPath = TEXT("/Script/Engine.DataTable'/Game/Table/InGame/EnemyTable.EnemyTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TPENEMY(*EnemyDataPath);
	TPCHECK(DT_TPENEMY.Succeeded());
	TPEnemyTable = DT_TPENEMY.Object;
	TPCHECK(TPEnemyTable->GetRowMap().Num() > 0);

	// �нú� ��ų ���̺� �ε�
	FString PassiveDataPath = TEXT("/Script/Engine.DataTable'/Game/Table/InGame/PassiveSkillTable.PassiveSkillTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_TPPASSIVE(*PassiveDataPath);
	TPCHECK(DT_TPPASSIVE.Succeeded());
	TPPassiveTable = DT_TPPASSIVE.Object;
	TPCHECK(TPPassiveTable->GetRowMap().Num() > 0);

	// ��ų ���̺� �ε�
	FString SkillDataPath = TEXT("/Script/Engine.DataTable'/Game/Table/InGame/SkillTable.SkillTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SKILL_TB(*SkillDataPath);
	TPCHECK(DT_SKILL_TB.Succeeded());
	TPSkillTable = DT_SKILL_TB.Object;
	TPCHECK(TPSkillTable->GetRowMap().Num() > 0);

	// �нú� ��ų ���̺� �ε�
	FString PassiveGroupDataPath = TEXT("/Script/Engine.DataTable'/Game/Table/InGame/PassiveGroupTable.PassiveGroupTable'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PASSIVE_GROUP_TB(*PassiveGroupDataPath);
	TPCHECK(DT_PASSIVE_GROUP_TB.Succeeded());
	TPPassiveGroupTable = DT_PASSIVE_GROUP_TB.Object;
	TPCHECK(TPPassiveGroupTable->GetRowMap().Num() > 0);
}

/*

	//-> �������� �Ŵ��� �߰�
	-> �������� ����
		-> ����� ���� �ε�
		-> Enemy ����
			-> ���̺��� �׷캰 ���� ����.
				-> Enemy Ÿ�Ժ� ���� ��ŭ �ε�.
				-> �׷캰 Enemy Spawn ������ ���� �ε�.
			-> ���� �׷��� �ƴ� Enemy���� ��Ȱ��ȭ.
		-> �÷��̾� ����.
	-> �������� Enemy �׷� üũ
		-> ���� �׷��� ���� �ð�(Ȥ�� �� �׷� Ŭ����)�� üũ, �����ؾߵǸ� Ȱ��ȭ.
	-> Next Stage
		-> ���� ���������� �ִٸ� �ε�.
		-> ���ٸ� ���� Ŭ����.
	-> �������� Ŭ����
		-> ���� �������� ���� ��� ���Ҵ��� üũ.
		-> ���� ������ �ǵ�� �ް� Character �ʿ� OnDead����� Recv�ް��ϱ�.


*/

void UTPStageManager::InitManager(class UTPGameInstance* InGameInstance)
{
	GameInstance = InGameInstance;
	SetManagerStep(EStageManagerStep::SMS_INIT);
}

void UTPStageManager::StartGame()
{
	//LoadLevel("Gameplay");

	SetManagerStep(EStageManagerStep::SMS_START_GAME);
}

void UTPStageManager::SetNextChapter()
{
	++CurrentChapterIndex;
	if(CurrentChapterIndex > ChapterIndexMax)
		CurrentChapterIndex = ChapterIndexMax;

	// ���̺��� ���� é���� �����͸� �����´�.

	FString ContextString = "Empty Chapter Data";
	FTPChapterTable* FindData = nullptr;
	for (const FName& RowName : TPChapterData->GetRowNames())
	{
		FindData = TPChapterData->FindRow<FTPChapterTable>(RowName, ContextString);
		if(FindData && FindData->Chapter == CurrentChapterIndex) // é�� �����ʹ� �ϳ��ۿ�����.
			break;
	}
	TPCHECK(FindData);
	CurChapterStages.Empty();
	int FindMaxStage = 0;
	if (FindData)
	{
		FindData->Chapter;
		StageIndexMax = FindData->MaxStage;

		FString StageContextString = "Empty Stage Data";
		for (const FName& RowName : TPStageData->GetRowNames())
		{
			FTPStageTable* FindStageData = TPStageData->FindRow<FTPStageTable>(RowName, StageContextString);
			if (FindStageData 
			&& FindStageData->Chapter == FindData->Chapter)
			{
				CurChapterStages.Add(*FindStageData);
				FindMaxStage = FMath::Max(FindMaxStage , FindStageData->Stage);
			}
		}
	}

	TPCHECK(CurChapterStages.Num()>0);
	TPCHECK(FindMaxStage == StageIndexMax);

	CurrentStageNum = 0;
	//StageIndexMax = CurChapterStages.Num()-1;
}

void UTPStageManager::CharacterLoad()
{
	if(IsExistMainCharacter())
		return;
	// ĳ���� �ε�
	FVector SpawnLocation = FVector(0, 0, 500); // ���ϴ� ��ġ
	FRotator SpawnRotation = FRotator::ZeroRotator;
	APlayerStart* CurStartPoint = GetStartPoint();
	if (CurStartPoint)
	{
		SpawnLocation = CurStartPoint->GetActorLocation();
		SpawnRotation = CurStartPoint->GetActorRotation();
	}
	ATPCharacter* NewChar = GameInstance->GetWorld()->SpawnActor<ATPCharacter>(LoadClass<ATPCharacter>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Characters/Blueprints/MainChar.MainChar_C'")), SpawnLocation, SpawnRotation);

	if (NewChar)
	{
		APlayerController* PlayerController = GameInstance->GetWorld()->GetFirstPlayerController();
		if (PlayerController)
			PlayerController->Possess(NewChar); // ĳ���� ���� ����
		auto TPPlayerState = Cast<ATPPlayerState>(PlayerController->PlayerState);
		TPCHECK(TPPlayerState != nullptr);
		TPPlayerState->InitPlayerData();

		NewChar->InitCharacter();
		NewChar->OnDead.AddLambda([this]()-> void {
			OnCharacterDead();
			});
		ArrPlayers.Add(NewChar);
	}
}

void UTPStageManager::SetNextStage()
{
	++CurrentStageNum;
	if(CurrentStageNum> StageIndexMax)
		CurrentStageNum =  StageIndexMax;

	// ���� �������� ����
	TPCHECK(CurChapterStages.Num() > CurrentStageNum);

	CurrentWaveIndex = 0;

	for (auto CurChapterStage : CurChapterStages)
	{
		if(CurChapterStage.Stage == CurrentStageNum)
			WaveIndexMax = CurChapterStage.Wave;
	}
	//WaveIndexMax = CurChapterStages[CurrentStageIndex].Wave;

	PrevStageUnload();

	if (StageGate)
	{
		StageGate->SetEnableCollision(false);
	}
}

void UTPStageManager::OpenGate()
{

	TPCHECK(CurStageGateSpawner != nullptr);

	if (StageGate != nullptr)
	{
		StageGate->SetEnableCollision(true);
		StageGate->SetActorTransform(CurStageGateSpawner->GetTransform());
	}
}

void UTPStageManager::PrevStageUnload()
{
	if (CurrentStageName.IsEmpty())
	{
		StageLoad();
	}
	APlayerController* PlayerController = GameInstance->GetWorld()->GetFirstPlayerController();
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;  // ���� ��ü�� Callback ������� ����
	LatentInfo.ExecutionFunction = FName("OnCompletedUnloadStage");  // ������ �Լ� ����
	LatentInfo.UUID = PlayerController->GetUniqueID();  // �⺻��
	LatentInfo.Linkage = LatentInfo.UUID;  // �⺻��

	UGameplayStatics::UnloadStreamLevel(
		GameInstance->GetWorld(),
		*CurrentStageName,
		LatentInfo, true
	);
}

void UTPStageManager::OnCompletedUnloadStage()
{
	StageLoad();
}

void UTPStageManager::StageLoad()
{

	FString NextLevelName = GetStageName();

	bool bSuc = false;


	CurrentStageName = NextLevelName;

	APlayerController* PlayerController = GameInstance->GetWorld()->GetFirstPlayerController();

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;  // ���� ��ü�� Callback ������� ����
	LatentInfo.ExecutionFunction = FName("OnCompletedLoadStage");  // ������ �Լ� ����
	LatentInfo.UUID = PlayerController->GetUniqueID();  // �⺻��
	LatentInfo.Linkage = LatentInfo.UUID;  // �⺻��

	UGameplayStatics::LoadStreamLevel(
		GameInstance->GetWorld(),
		*NextLevelName,
		true,  // �ε� �� �ڵ� Ȱ��ȭ
		false, // Should Block on Load (false�� �����Ͽ� �񵿱� �ε�)
		LatentInfo
	);
}

void UTPStageManager::OnCompletedLoadStage()
{
	CharacterLoad();

	// GetSpawnObj
	CurStageSpawner.Empty();
	CurStageGateSpawner = nullptr;

	if (StageGate == nullptr)
	{
		FString GatePath = "/Script/Engine.Blueprint'/Game/Blueprints/BP_StageGate.BP_StageGate_C'";
		StageGate = GameInstance->GetWorld()->SpawnActor<ATPStageGate>(LoadClass<ATPStageGate>(nullptr, *GatePath));
		TPCHECK(StageGate != nullptr);
		if (StageGate != nullptr)
			StageGate->SetEnableCollision(false);
	}

	for (ULevel* Level : GameInstance->GetWorld()->GetLevels())
	{
		if (Level && Level->GetOuter()->GetName().Contains(*CurrentStageName))
		{
			for (AActor* Actor : Level->Actors)
			{
				if (!Actor) continue;
				ATPEnemySpawner* CurSpawner = Cast<ATPEnemySpawner>(Actor);
				if(CurSpawner == nullptr)
					continue;
				if(CurSpawner->CurSpawnerType == ESpawnerType::EST_ENEMY)
					CurStageSpawner.Add(CurSpawner->GetSpawnIndex(),CurSpawner);
				else if(CurSpawner->CurSpawnerType == ESpawnerType::EST_GATE)
					CurStageGateSpawner = CurSpawner;
			}
		}
	}
	NextWave();
}

void UTPStageManager::NextWave()
{
	for (auto CurEnemy : ArrEnemies)
	{
		CurEnemy->Destroy();
	}
	ArrEnemies.Empty();

	++CurrentWaveIndex;
	if(CurrentWaveIndex>WaveIndexMax)
		CurrentWaveIndex = WaveIndexMax;

	// Enemy Spawn & Set
	int FindIndex = -1;
	for (int CurIndex = 0; CurIndex < CurChapterStages.Num(); ++CurIndex)
	{
		if(CurChapterStages[CurIndex].Stage != CurrentStageNum || CurChapterStages[CurIndex].Wave != CurrentWaveIndex)
			continue;
		FindIndex = CurIndex;
		break;
	}

	TPCHECK(FindIndex != -1);

	CurrentStageIndex = FindIndex;
	FTPStageTable& CurStageInfo = CurChapterStages[CurrentStageIndex];
	for (auto EnemySpawnInfo : CurStageInfo.SpawnEnemies)
	{
		FTPEnemyData* CurEnemyData = GetTPEnemyData(EnemySpawnInfo.EnemyType);

		for (int SpawnCount = 0; SpawnCount < EnemySpawnInfo.Count; ++SpawnCount)
		{
			int RandSpawnIndex = FMath::RandRange(1, CurStageInfo.SpawnPoint.Num() - 1);
			TPCHECK(CurStageSpawner.Find(RandSpawnIndex));
			FVector SpawnPoint = CurStageSpawner[RandSpawnIndex]->GetSpawnPoint();

			FRotator SpawnRotation = FRotator::ZeroRotator;
			ATPCharacter* NewEnemy = GameInstance->GetWorld()->SpawnActor<ATPCharacter>(LoadClass<ATPCharacter>(nullptr, *CurEnemyData->BPEnemyPath), SpawnPoint, SpawnRotation);

			if (NewEnemy)
			{
				NewEnemy->InitCharacter();
				NewEnemy->OnDead.AddLambda([this]()-> void {
					OnCharacterDead();
					});
				ArrEnemies.Add(NewEnemy);
			}
		}
	}

	SetManagerStep( EStageManagerStep::SMS_PLAY_STAGE);
}

void UTPStageManager::PlayStage()
{
	for (auto CurPlayer : ArrPlayers)
		CurPlayer->PlayStage();
	for (auto CurEnemy : ArrEnemies)
		CurEnemy->PlayStage();
}

void UTPStageManager::GameEnd()
{
	ArrPlayers.Empty();
	ArrEnemies.Empty();
	GetMainCharacter()[0]->RestartLevel();
}

class APlayerStart* UTPStageManager::GetStartPoint()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GameInstance->GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	if (PlayerStarts.Num() > 0)
	{
		// ù ��° PlayerStart ��ȯ (�������� ���������� FMath::RandRange ���)
		return Cast<APlayerStart>(PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)]);
	}
	return nullptr;
}

// FTPSkillInfo UTPStageManager::GetSkillInfo(FTPSkillInitData& InitData)
// {
// 	// ������ ���̺��� �����.
// 	FTPSkillInfo ResultData;
// 	ResultData.SkillIndex = InitData.SkillIndex;
// 	ResultData.SkillLv = InitData.SkillLv;
// 	ResultData.ColdTime = 0.f;
// 	ResultData.CurrentColdTime = 0.f;
// 	ResultData.SkillType = ESkillType::PASSIVE;
// 	ResultData.BufType = EBufType::BT_ADD_STAT;
// 	ResultData.BufValue = FMath::RandRange(1, 10);
// 	ResultData.BufStatType = (EBufStatType)FMath::RandRange(1, (int)(EBufStatType::BST_MAX)-1);
// 	switch (ResultData.SkillIndex)
// 	{
// 	 case 1:
// 		 ResultData.BufType = EBufType::BT_ADD_STAT;
// 		 break;
// 	 case 2:
// 		 ResultData.BufType = EBufType::BT_MUL_STAT;
// 		 break;
// 	default:
//		break;
// 	}
// 
// 	return ResultData;
// }

UTPSkillBase_Legacy* UTPStageManager::GetSkillInfo_Legacy(FTPSkillInitData& InitData)
{
	// Table
	if (InitData.SkillType == ESkillType::ST_PASSIVE)
	{
		FString ContextString = "Empty Passive Data";
		FTPPassiveSkillTable* FindData = nullptr;
		for (const FName& RowName : TPPassiveTable->GetRowNames())
		{
			FindData = TPPassiveTable->FindRow<FTPPassiveSkillTable>(RowName, ContextString);
			if (FindData && FindData->Index == InitData.SkillIndex) // é�� �����ʹ� �ϳ��ۿ�����.
				break;
		}
		if(FindData != nullptr)
		{
			UClass* SkillActorClass = LoadClass<UTPSkillBase_Legacy>(nullptr, *FindData->BPSkillPath);
			
			UTPSkillBase_Legacy* SkillObject = NewObject<UTPSkillBase_Legacy>(GameInstance, SkillActorClass);

			return SkillObject;
			//return GameInstance->GetWorld()->SpawnActor<UTPSkillBase>(SkillActorClass);
		}

	}
	else
	{

	}
	

	return nullptr;
}

UTPSkillController* UTPStageManager::GetSkillController(FTPSkillInitData& InitData, UTPSkillComponent* SkillComp)
{
 	UTPSkillController* NewSkillObj = NewObject<UTPSkillController>();
	if (NewSkillObj)
	{
		NewSkillObj->InitSkill(InitData, SkillComp, this);
		return NewSkillObj;
	}
	return nullptr;
}


FTPSkillTable* UTPStageManager::GetSkillInfo(int InSkillIndex)
{
	FString ContextString = "Empty SkillInfo Data";
	FTPSkillTable* FindData = nullptr;
	for (const FName& RowName : TPSkillTable->GetRowNames())
	{
		FindData = TPSkillTable->FindRow<FTPSkillTable>(RowName, ContextString);
		if (FindData && FindData->Index == InSkillIndex) // �����ʹ� �ϳ��ۿ�����.
			return FindData;
	}
	return nullptr;
}

TArray<FTPPassiveGroupTable*> UTPStageManager::GetPassiveSkillInfo(int InGroupID, int InLv)
{
	FString ContextString = "Empty Passive Skill Group Data";
	TArray<FTPPassiveGroupTable*> ArrFindData;
	for (const FName& RowName : TPPassiveGroupTable->GetRowNames())
	{
		FTPPassiveGroupTable* FindData = TPPassiveGroupTable->FindRow<FTPPassiveGroupTable>(RowName, ContextString);
		if (FindData && FindData->GroupID == InGroupID) // �����ʹ� �ϳ��ۿ�����.
		{
			if (FindData->Lv == InLv)
				ArrFindData.Add(FindData);
			else if (FindData->Lv > InLv)
				break;
		}
	}
	return ArrFindData;
}

ESkillEffectType UTPStageManager::GetEffectType(ESkillType InSkillType, int InGroupID, int InLv)
{
	switch (InSkillType)
	{
	case ESkillType::ST_PASSIVE:
	{
		TArray<FTPPassiveGroupTable*> CurFindInfo = GetPassiveSkillInfo(InGroupID, InLv);
		TPCHECK(CurFindInfo.Num()>0, ESkillEffectType::SEffect_NONE);
		return CurFindInfo[0]->EffectType;
	}
	break;
	case ESkillType::ST_ACTIVE:
	{

	}
	break;
	default:
		break;
	}
	return ESkillEffectType::SEffect_NONE;
}

void UTPStageManager::SetManagerStep(EStageManagerStep NewStep)
{
	EStageManagerStep NextStep = EStageManagerStep::SMS_NONE;

	CurrentStep = NewStep;
	switch (CurrentStep)
	{
	case UTPStageManager::EStageManagerStep::SMS_INIT:
	{
		// ���� ���� �¾�
	}
		break;
	case UTPStageManager::EStageManagerStep::SMS_START_GAME:
	{
		CurrentChapterIndex = 0;
		ChapterIndexMax = TPChapterData->GetRowMap().Num();
		CurrentStageNum = 0;
		CurrentStageIndex = 0;
		StageIndexMax = 0;
		CurrentWaveIndex = 0;
		WaveIndexMax = 0;
		CurrentStageName = "";

		ArrEnemies.Empty();
		ArrPlayers.Empty();

		NextStep = EStageManagerStep::SMS_INIT_CHAPTER;
	}
		break;
	case UTPStageManager::EStageManagerStep::SMS_INIT_CHAPTER:
	{
		// ���� �������� ��ε�....
		SetNextChapter();
		NextStep = EStageManagerStep::SMS_LOAD_STAGE;
	}
		break;
	case UTPStageManager::EStageManagerStep::SMS_LOAD_STAGE:
	{
		if (CheckChapterEnd())
		{
			// Chapter Game End
			GameEnd();
		}
		else
			SetNextStage();
	}
		break;
	case UTPStageManager::EStageManagerStep::SMS_PLAY_STAGE:
	{
		PlayStage();
	}
		break;
	case UTPStageManager::EStageManagerStep::SMS_END_STAGE:
		break;
	case UTPStageManager::EStageManagerStep::SMS_END_CHAPTER:
		break;
	case UTPStageManager::EStageManagerStep::SMS_END_GAME:
		break;
	default:
		break;
	}
	if(NextStep != EStageManagerStep::SMS_NONE)
		SetManagerStep(NextStep);
}

FString UTPStageManager::GetStageName()
{
	for (auto CurChapterStage : CurChapterStages)
	{
		if (CurChapterStage.Stage == CurrentStageNum)
			return CurChapterStage.StageName;
	}
	return "";
}

void UTPStageManager::OnCharacterDead()
{
	int DeadPlayer =0;
	for (auto CurPlayer : ArrPlayers)
	{
		if(CurPlayer->GetCharacterState() == ECharacterState::DEAD)
			++DeadPlayer;
	}
	if (ArrPlayers.Num() == DeadPlayer)
	{
		//  Game Over
		GameEnd();
	}

	if (CheckWaveEnd())
	{
		// Next Stage�� ����?
		if (CheckStageEnd())
		{
			OpenGate();
			//SetManagerStep(EStageManagerStep::SMS_LOAD_STAGE);
		}
		else
		{
			// Next Wave�� ����?
			NextWave();
		}
	}
}

bool UTPStageManager::CheckWaveEnd()
{
	int DeadEnemies = 0;
	for (auto CurEnemy : ArrEnemies)
	{
		if (CurEnemy->GetCharacterState() == ECharacterState::DEAD)
			++DeadEnemies;
	}
	if (ArrEnemies.Num() == DeadEnemies)
	{
		// Wave Over
		return true;
	}
	return false;
}

bool UTPStageManager::CheckStageEnd()
{
	if(CurrentWaveIndex >= WaveIndexMax)
		return true;
	
	return false;
}

bool UTPStageManager::CheckChapterEnd()
{
	if (CurrentStageNum >= StageIndexMax)
		return true;

	return false;
}

struct FTPCharacterData* UTPStageManager::GetTPCharacterData(int32 InLevel)
{
	return TPCharacterTable->FindRow<FTPCharacterData>(*FString::FromInt(InLevel), TEXT(""));
}

TArray<class ATPCharacter*> UTPStageManager::GetMainCharacter()
{
	return ArrPlayers;
}

bool UTPStageManager::IsExistMainCharacter()
{
	return ArrPlayers.Num() > 0;
}

struct FTPEnemyData* UTPStageManager::GetTPEnemyData(int32 InIndex)
{
	return TPEnemyTable->FindRow<FTPEnemyData>(*FString::FromInt(InIndex), TEXT(""));
}

TArray<class ATPCharacter*> UTPStageManager::GetEnemies()
{
	return ArrEnemies;
}
