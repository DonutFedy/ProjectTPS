
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "Components/ActorComponent.h"
#include "TPCharacterStatComponent.generated.h"

// 스킬 발동에 의한 버프 스탯 정보
USTRUCT(BlueprintType)
struct FTPCharacterStatInfo
{
	GENERATED_BODY()
public:
	FTPCharacterStatInfo()
	{
		//ArrStat.Init(0.f, (int)EBufStatType::BST_MAX);
		MapBasicStat.Empty();
		MapAddStat.Empty();
	}

	void InitStatInfo()
	{
// 		for (float& Elem : ArrStat)
// 		{
// 			Elem = 0.f;
// 		}
		MapBasicStat.Empty();
		MapAddStat.Empty();
	}

	float GetBasicStat(EBufStatType InStatType)
	{
		if (float* FoundValue = MapBasicStat.Find(InStatType))
		{
			return *FoundValue;
		}
		return 0.f;
	}
	void SetBasicStat(EBufStatType InStatType, float InStatValue)
	{
		MapBasicStat.FindOrAdd(InStatType, InStatValue);
	}

	float GetAddStat(EBufStatType InStatType)
	{
		if (float* FoundValue = MapAddStat.Find(InStatType))
		{
			return *FoundValue;
		}
		return 0.f;
	}
	void SetAddStat(EBufStatType InStatType, float InStatValue)
	{
		MapAddStat.FindOrAdd(InStatType, InStatValue);
	}

	void Add_AddStat(EBufStatType InStatType, float InStatValue)
	{
		if (float* FoundValue = MapAddStat.Find(InStatType))
			*FoundValue += InStatValue;
		else
			SetAddStat(InStatType, InStatValue);
	}
	void Mul_AddStat(EBufStatType InStatType, float InStatValue)
	{
		if (float* FoundValue = MapAddStat.Find(InStatType))
			*FoundValue *= InStatValue;
		else
			SetAddStat(InStatType, InStatValue);
	}

	float GetStat(EBufStatType InStatType)
	{
// 		switch (InStatType)
// 		{
// 		case EBufStatType::BST_BULLET_SPD:
// 		case EBufStatType::BST_RELOAD_SPD:
// 		case EBufStatType::BST_ACCURACY:
// 		case EBufStatType::BST_RECOIL_DOWN:
// 		case EBufStatType::BST_DODGE_STAMINA_DOWN:
// 		case EBufStatType::BST_INCREASED_BULLET_AMMO:
// 		case EBufStatType::BST_SKILL_COOLTIME:
// 		{
// 			return GetAddStat(InStatType) * GetBasicStat(InStatType);
// 		}
// 		default:
// 			break;
// 		}
		return GetAddStat(InStatType) + GetBasicStat(InStatType);
	}

public:
	
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	TMap<EBufStatType, float> MapBasicStat;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	TMap<EBufStatType, float> MapAddStat;

	
};

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnShieldChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStaminaChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSomethingChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTPS_API UTPCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;

	void CallProcessBufStat(EBufStatType InBufType);

private:
	float GetHPMax();
	float GetHPRecover();
	float GetHPRest();
	float GetStaminaMax();
	float GetShieldMax();
	float GetShieldRecover();
	float GetShieldRest();

public:
	void ResetAll();
	void SetStatComponent(int32 InNewLevel, class ATPCharacter* CurrentChar);
	void SetAI(int32 InAIIndex, class ATPCharacter* CurrentChar);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	void SetStamina(float NewStamina);
	void RecoveryStamina(float DeltaTime);
	bool UseStamina(float UsingStamina);
	void SetShield(float NewShield);
	void RecoveryShield(float DeltaTime);
	void RecoveryHP(float DeltaTime);
	bool UseShield(float UsingShield);

	float GetReloadSpd();

	float GetAttack();
	float GetAttackRange();
	float GetFinalCriticalDamageRate();
	float GetFinalCriticalRate();
	float GetFinalPierceRate();
	float GetHPRatio();
	float GetStaminaRatio();
	float GetShieldRatio();
	int32 GetDropExp();

	FTPCharacterStatInfo& GetStatInfo(){return CurStatInfo;}



	float GetFireRate();
	float GetAccuracy();
	float GetAccuracyRecovery();
	float GetAccuracyDown();
	float GetAccuracyMin();
	float GetRecoil();
	float GetBulletSpd();
	float GetDefencePoint();
	void SetCurWeapon(TWeakObjectPtr<class ATPWeapon> InNewWeapon);

	bool GetIsAttacking() {return IsAttacking;}
	void SetIsAttacking(bool InIsAttacking){IsAttacking = InIsAttacking;}

	bool GetIsReloading() { return IsReloading; }
	void SetIsReloading(bool InIsReloading) { IsReloading = InIsReloading; }

	bool GetIsHip() { return IsHip; }
	void SetIsHip(bool InIsHip);
	
	float GetAccuracyRatio();
	float GetCurrentAccuracyRate() { return CurrentAccuracyRate; }
	float GetMaxAccuracyRate() { return MaxAccuracyRate; }
	float GetCurAccuracyRecovery() { return CurAccuracyRecovery; }
	float GetCurAccuracyDown() { return CurAccuracyDown; }

	float GetJumpForce();
	float GetMoveSpd();

	void RecoverAccuracy(float DeltaTime);
	void DecreaseAccuracy();

	TObjectPtr<class ATPCharacter> GetOwnChar(){return OwnChar;}

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;
	FOnShieldChangedDelegate OnShieldChanged;
	FOnStaminaChangedDelegate OnStaminaChanged;
	FOnSomethingChangedDelegate OnAccuracyChanged;

private:

	UPROPERTY(EditInstanceOnly, Category=Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;


#pragma region Current Stat / Transient

	//=======================저장 필요없는 현재 상황======================

	UPROPERTY(Transient, VisibleInstanceOnly, Category=Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float HPRestTime;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float StaminaRestTime;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float ShieldRestTime;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentStamina;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentShield;


	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess = true))
	bool IsReloading;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Accuracy, Meta = (AllowPrivateAccess = true))
	bool IsHip;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category=Accuracy, Meta = (AllowPrivateAccess = true))
	float CurrentAccuracyRate;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Accuracy, Meta = (AllowPrivateAccess = true))
	float MaxAccuracyRate;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Accuracy, Meta = (AllowPrivateAccess = true))
	float CurAccuracyRecovery;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Accuracy, Meta = (AllowPrivateAccess = true))
	float CurAccuracyDown;


	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	float Defence;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = AddStat, Meta = (AllowPrivateAccess = true))
	FTPCharacterStatInfo CurStatInfo;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	TWeakObjectPtr< class ATPWeapon> CurrentWeapon;
	
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	bool IsPlayer;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Damage, Meta = (AllowPrivateAccess = true))
	class ATPCharacter* OwnChar;
#pragma endregion

};
