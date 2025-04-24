// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCharacterStatComponent.h"
#include "TPGameInstance.h"
#include "Table/TPCharacterData.h"
#include "Table/TPEnemyData.h"
#include "TPCharacter.h"
#include "TPWeapon.h"

// Sets default values for this component's properties
UTPCharacterStatComponent::UTPCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


	bWantsInitializeComponent = true;
	// ...

	ResetAll();
}


// Called when the game starts
void UTPCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UTPCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTPCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UTPCharacterStatComponent::CallProcessBufStat(EBufStatType InBufType)
{
	// 갱신된 스탯의 타입에 따라 후처리해줘야하는 부분을...
	switch (InBufType)
	{
	case EBufStatType::BST_SHIELD_MAX:
		OnShieldChanged.Broadcast();
		break;
	case EBufStatType::BST_HEALTH_MAX:
		OnHPChanged.Broadcast();
		break;
	case EBufStatType::BST_STAMINA_MAX:
		OnStaminaChanged.Broadcast();
		break;
	case EBufStatType::BST_MOVE_SPD:
		OwnChar->GetCharacterMovement()->MaxWalkSpeed = CurStatInfo.GetStat(EBufStatType::BST_JUMP_FORCE);
		break;
	case EBufStatType::BST_RECOIL:
		break;
	case EBufStatType::BST_RECOIL_DOWN:
		break;
	case EBufStatType::BST_DODGE_INVINCIBILITY_TIME:
		break;
	case EBufStatType::BST_DODGE_STAMINA_DOWN:
		break;
	case EBufStatType::BST_INCREASED_BULLET_AMMO:
		break;
	case EBufStatType::BST_INCREASED_BULLET_CHARGE:
		break;
	case EBufStatType::BST_SKILL_COOLTIME:
		break;
	case EBufStatType::BST_DECREASED_SKILL_COOLTIME:
		break;
	case EBufStatType::BST_INCREASED_SKILL_STACK_COUNT:
		break;
	case EBufStatType::BST_INCREASED_REWARD_RATE:
		break;
	case EBufStatType::BST_DECREASE_ENEMY_STAT_RATE:
		break;
	case EBufStatType::BST_JUMP_FORCE:
		OwnChar->GetCharacterMovement()->JumpZVelocity = CurStatInfo.GetStat(EBufStatType::BST_JUMP_FORCE) * 500;
		break;
	case EBufStatType::BST_MAX:
		break;
	default:
		break;
	}
}

float UTPCharacterStatComponent::GetHPMax()
{
// 	if(IsPlayer)
// 		return CurrentStatData->HpMax;
// 	else
// 		return CurrentEnemyData->HpMax;
	return CurStatInfo.GetStat( EBufStatType::BST_HEALTH_MAX );
}

float UTPCharacterStatComponent::GetHPRecover()
{
	return CurStatInfo.GetStat(EBufStatType::BST_HEALTH_RECOVER);
}

float UTPCharacterStatComponent::GetHPRest()
{
	return CurStatInfo.GetStat(EBufStatType::BST_HEALTH_REST);
}

float UTPCharacterStatComponent::GetStaminaMax()
{
// 	if (IsPlayer)
// 		return CurrentStatData->StaminaMax;
// 	return 0.f;

	return CurStatInfo.GetStat(EBufStatType::BST_STAMINA_MAX);
}

float UTPCharacterStatComponent::GetShieldMax()
{
// 	if (IsPlayer)
// 		return CurrentStatData->ShieldMax;
// 	else
// 		return CurrentEnemyData->ShieldMax;
	return CurStatInfo.GetStat(EBufStatType::BST_SHIELD_MAX);
}

float UTPCharacterStatComponent::GetShieldRecover()
{
// 	if (IsPlayer)
// 		return CurrentStatData->ShieldRecover;
// 	else
// 		return CurrentEnemyData->ShieldRecover;
	return CurStatInfo.GetStat(EBufStatType::BST_SHIELD_MAX);
}

float UTPCharacterStatComponent::GetShieldRest()
{
// 	if (IsPlayer)
// 		return CurrentStatData->ShieldRest;
// 	else
// 		return CurrentEnemyData->ShieldRest;
	return CurStatInfo.GetStat(EBufStatType::BST_SHIELD_REST);
}

void UTPCharacterStatComponent::ResetAll()
{
	Level = 1;
	StaminaRestTime = 0;
	ShieldRestTime = 0;

	IsAttacking = false;
	IsReloading = false;

	CurStatInfo.InitStatInfo();
	Defence = 0.f;
}

void UTPCharacterStatComponent::SetStatComponent(int32 InNewLevel, ATPCharacter* CurrentChar)
{
	OwnChar = CurrentChar;
	auto TPGameInstance = Cast<UTPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	IsPlayer = true;

	TPCHECK(TPGameInstance != nullptr);
	FTPCharacterData* CurrentStatData = TPGameInstance->GetTPCharacterData(InNewLevel);
	if (CurrentStatData != nullptr)
	{

		CurStatInfo.SetBasicStat(EBufStatType::BST_JUMP_FORCE , CurrentStatData->JumpForce);
		CurStatInfo.SetBasicStat(EBufStatType::BST_CRITICAL_RATE , CurrentStatData->CriticalRate);
		CurStatInfo.SetBasicStat(EBufStatType::BST_CRITICAL_DAMAGE , CurrentStatData->CriticalDamage);
		CurStatInfo.SetBasicStat(EBufStatType::BST_RELOAD_SPD , CurrentStatData->ReloadSpd);
		CurStatInfo.SetBasicStat(EBufStatType::BST_BULLET_SPD , CurrentStatData->BulletSpd);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SHIELD_MAX, CurrentStatData->ShieldMax);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SHIELD_REST, CurrentStatData->ShieldRest);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SHIELD_RECOVER, CurrentStatData->ShieldRecover);
		CurStatInfo.SetBasicStat(EBufStatType::BST_HEALTH_MAX, CurrentStatData->HpMax);
		CurStatInfo.SetBasicStat(EBufStatType::BST_HEALTH_REST, CurrentStatData->HpRest);
		CurStatInfo.SetBasicStat(EBufStatType::BST_HEALTH_RECOVER, CurrentStatData->HpRecover);
		CurStatInfo.SetBasicStat(EBufStatType::BST_STAMINA_MAX, CurrentStatData->StaminaMax);
		CurStatInfo.SetBasicStat(EBufStatType::BST_STAMINA_REST, CurrentStatData->StaminaRest);
		CurStatInfo.SetBasicStat(EBufStatType::BST_STAMINA_RECOVER, CurrentStatData->StaminaRecover);
		CurStatInfo.SetBasicStat(EBufStatType::BST_ATTACK, CurrentStatData->Attack);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SKILL_ATTACK, CurrentStatData->SkillAttack);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SKILL_COOLTIME, CurrentStatData->SkillCoolTime);
		CurStatInfo.SetBasicStat(EBufStatType::BST_DEFENCE, CurrentStatData->Defence);
		CurStatInfo.SetBasicStat(EBufStatType::BST_PIERCE, CurrentStatData->Pierce);
		CurStatInfo.SetBasicStat(EBufStatType::BST_RECOIL, CurrentStatData->Recoil);
		CurStatInfo.SetBasicStat(EBufStatType::BST_ACCURACY, CurrentStatData->Accuracy);
		CurStatInfo.SetBasicStat(EBufStatType::BST_MOVE_SPD, CurrentStatData->MoveSpd);
		CurStatInfo.SetBasicStat(EBufStatType::BST_FIRE_RATE, CurrentStatData->FireRate);

		Level = InNewLevel;
		SetHP(GetHPMax());
		SetShield(GetShieldMax());
		SetStamina(GetStaminaMax());
		
		CurrentChar->GetCharacterMovement()->JumpZVelocity = CurStatInfo.GetStat( EBufStatType::BST_JUMP_FORCE) * 500;
	}
	else
	{
		TPLOG(Error, TEXT("Level (%d) data doesn't exist"), InNewLevel);
	}
}


void UTPCharacterStatComponent::SetAI(int32 InAIIndex, ATPCharacter* CurrentChar)
{
	OwnChar = CurrentChar;
	auto TPGameInstance = Cast<UTPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	IsPlayer = false;
	TPCHECK(TPGameInstance != nullptr);
	FTPEnemyData* CurrentEnemyData = TPGameInstance->GetTPEnemyData(InAIIndex);
	if (CurrentEnemyData != nullptr)
	{
		Level = CurrentEnemyData->Level;
		CurStatInfo.SetBasicStat(EBufStatType::BST_JUMP_FORCE, CurrentEnemyData->JumpForce);
		CurStatInfo.SetBasicStat(EBufStatType::BST_CRITICAL_RATE, CurrentEnemyData->CriticalRate);
		CurStatInfo.SetBasicStat(EBufStatType::BST_CRITICAL_DAMAGE, CurrentEnemyData->CriticalDamage);
		CurStatInfo.SetBasicStat(EBufStatType::BST_RELOAD_SPD, CurrentEnemyData->ReloadSpd);
		CurStatInfo.SetBasicStat(EBufStatType::BST_BULLET_SPD, CurrentEnemyData->BulletSpd);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SHIELD_MAX, CurrentEnemyData->ShieldMax);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SHIELD_REST, CurrentEnemyData->ShieldRest);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SHIELD_RECOVER, CurrentEnemyData->ShieldRecover);
		CurStatInfo.SetBasicStat(EBufStatType::BST_HEALTH_MAX, CurrentEnemyData->HpMax);
		CurStatInfo.SetBasicStat(EBufStatType::BST_HEALTH_REST, CurrentEnemyData->HpRest);
		CurStatInfo.SetBasicStat(EBufStatType::BST_HEALTH_RECOVER, CurrentEnemyData->HpRecover);
		CurStatInfo.SetBasicStat(EBufStatType::BST_ATTACK, CurrentEnemyData->Attack);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SKILL_ATTACK, CurrentEnemyData->SkillAttack);
		CurStatInfo.SetBasicStat(EBufStatType::BST_SKILL_COOLTIME, CurrentEnemyData->SkillCoolTime);
		CurStatInfo.SetBasicStat(EBufStatType::BST_DEFENCE, CurrentEnemyData->Defence);
		CurStatInfo.SetBasicStat(EBufStatType::BST_PIERCE, CurrentEnemyData->Pierce);
		CurStatInfo.SetBasicStat(EBufStatType::BST_ACCURACY, CurrentEnemyData->Accuracy);
		CurStatInfo.SetBasicStat(EBufStatType::BST_MOVE_SPD, CurrentEnemyData->MoveSpd);
		CurStatInfo.SetBasicStat(EBufStatType::BST_FIRE_RATE, CurrentEnemyData->FireRate);
		CurStatInfo.SetBasicStat(EBufStatType::BST_DROP_EXP, CurrentEnemyData->DropExp);
		CurStatInfo.SetBasicStat(EBufStatType::BST_DROP_GROUP, CurrentEnemyData->DropGroup);
		CurStatInfo.SetBasicStat(EBufStatType::BST_ATTACK_RANGE, CurrentEnemyData->AttackRange);


		SetHP(GetHPMax());
		SetShield(GetShieldMax());

		CurrentChar->GetCharacterMovement()->JumpZVelocity = CurStatInfo.GetStat(EBufStatType::BST_JUMP_FORCE) * 500;
	}
	else
	{
		TPLOG(Error, TEXT("Enemy Index (%d) data doesn't exist"), InAIIndex);
	}
}

void UTPCharacterStatComponent::SetDamage(float NewDamage)
{
	StaminaRestTime = CurStatInfo.GetStat(EBufStatType::BST_STAMINA_REST);
	ShieldRestTime = GetShieldRest();
	HPRestTime = GetHPRest();

	float CurDamage = NewDamage;
	CurDamage -= CurrentShield;
	SetShield(FMath::Clamp<float>(CurrentShield - NewDamage, 0.f, GetShieldMax()));


	if(CurDamage > 0.f)
		SetHP(FMath::Clamp<float>(CurrentHP - CurDamage, 0.f, GetHPMax()));
}

void UTPCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnHPIsZero.Broadcast();
	}
}

void UTPCharacterStatComponent::SetStamina(float NewStamina)
{
	CurrentStamina = NewStamina;
	if (CurrentStamina <= KINDA_SMALL_NUMBER)
	{
		CurrentStamina = 0.f;
	}
	else if (GetStaminaMax() < CurrentStamina)
	{
		CurrentStamina = GetStaminaMax();
	}
	OnStaminaChanged.Broadcast();
}

void UTPCharacterStatComponent::RecoveryStamina(float DeltaTime)
{
	if (StaminaRestTime > 0)
	{
		StaminaRestTime -= DeltaTime;
		return;
	}
	StaminaRestTime = 0.f;
	//StaminaRestTime = CurStatInfo.GetStat(EBufStatType::BST_STAMINA_REST);

	CurrentStamina += CurStatInfo.GetStat(EBufStatType::BST_STAMINA_RECOVER);
	if(GetStaminaMax() < CurrentStamina)
	{
		CurrentStamina = GetStaminaMax();
	}
	OnStaminaChanged.Broadcast();
}

bool UTPCharacterStatComponent::UseStamina(float UsingStamina)
{
	if (CurrentStamina - UsingStamina < 0)
		return false;
	CurrentStamina -= UsingStamina;
	if (CurrentStamina <= KINDA_SMALL_NUMBER)
	{
		CurrentStamina = 0.f;
	}
	OnStaminaChanged.Broadcast();
	return true;
}

void UTPCharacterStatComponent::SetShield(float NewShield)
{
	CurrentShield = NewShield;
	if (CurrentShield <= KINDA_SMALL_NUMBER)
	{
		CurrentShield = 0.f;
	}
	else if (GetShieldMax() < CurrentShield)
	{
		CurrentShield = GetShieldMax();
	}
	OnShieldChanged.Broadcast();
}

void UTPCharacterStatComponent::RecoveryShield(float DeltaTime)
{
	if (ShieldRestTime > 0)
	{
		ShieldRestTime -= DeltaTime;
		return;
	}
	ShieldRestTime = 0.f;
	//ShieldRestTime = GetShieldRest();

	CurrentShield += GetShieldRecover();
	if (GetShieldMax() < CurrentShield)
	{
		CurrentShield = GetShieldMax();
	}
	OnShieldChanged.Broadcast();
}

void UTPCharacterStatComponent::RecoveryHP(float DeltaTime)
{
	if (HPRestTime > 0)
	{
		HPRestTime -= DeltaTime;
		return;
	}
	HPRestTime=0.f;
	//HPRestTime = GetHPRest();

	CurrentHP += GetHPRecover();
	if (GetHPMax() < CurrentHP)
	{
		CurrentHP = GetHPMax();
	}
	OnHPChanged.Broadcast();
}

bool UTPCharacterStatComponent::UseShield(float UsingShield)
{
	if(CurrentShield - UsingShield < 0)
		return false;

	CurrentShield -= UsingShield;
	if (CurrentShield <= KINDA_SMALL_NUMBER)
	{
		CurrentShield = 0.f;
	}
	OnShieldChanged.Broadcast();
	return true;
}


float UTPCharacterStatComponent::GetAttack()
{
	if (IsPlayer)
	{
		if(CurrentWeapon != nullptr)
			return (1 + CurStatInfo.GetStat(EBufStatType::BST_ATTACK_RATE)) * (CurrentWeapon->GetAttackDamage() + CurStatInfo.GetStat(EBufStatType::BST_WEAPON_ATTACK) + CurStatInfo.GetStat(EBufStatType::BST_ATTACK)); /*+ 추가 공격력 배율*/
	}
	else
	{
		return CurStatInfo.GetStat(EBufStatType::BST_ATTACK);
	}
	return 0.f;
}

float UTPCharacterStatComponent::GetAttackRange()
{
	if (IsPlayer)
	{
		return 80.f;
	}
	else
	{
		//float AttackRange = CurrentEnemyData->AttackRange;
		float AttackRange = CurStatInfo.GetStat(EBufStatType::BST_ATTACK_RANGE);
		if(CurrentWeapon->GetAnimAttackType() == EAnimAttackType::NEAR_ATTACK)
			AttackRange = 80.f;
		return AttackRange;
	}
}

float UTPCharacterStatComponent::GetFinalCriticalDamageRate()
{
//치명타 피해 계산 = (무기 치명타 피해 + 추가 치명타 피해) * (캐릭터 치명타 피해 배율 + 추가 치명타 피해 배율)				
	if (IsPlayer)
	{
		if (CurrentWeapon != nullptr)
			return (CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE_RATE)) * (CurrentWeapon->GetCriticalDamage() + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE) ); /*+ 추가 공격력 배율*/
			//return (CurrentStatData->CriticalDamage + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE_RATE)) * (CurrentWeapon->GetCriticalDamage() + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE) ); /*+ 추가 공격력 배율*/
	}
	else
	{
		if (CurrentWeapon != nullptr)
			return (CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE_RATE) ) * (CurrentWeapon->GetCriticalDamage() + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE) ); /*+ 추가 공격력 배율*/
			//return (CurrentEnemyData->CriticalDamage + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE_RATE) ) * (CurrentWeapon->GetCriticalDamage() + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE) ); /*+ 추가 공격력 배율*/
	}
	return 0.f;
}

float UTPCharacterStatComponent::GetFinalCriticalRate()
{
//치명타 확률 계산 = (무기 치명타 확률 + 추가 치명타 확률) * (1+(캐릭터 치명타 확률 배율 + 추가 치명타 확률 배율))				
	if (IsPlayer)
	{
		if (CurrentWeapon != nullptr)
			return (1 + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_RATE) ) * (CurrentWeapon->GetCriticalRate() + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE) );
			//return (1 + CurrentStatData->CriticalRate + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_RATE) ) * (CurrentWeapon->GetCriticalRate() + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL) );
	}
	else
	{
		if (CurrentWeapon != nullptr)
			return (1 + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_RATE) ) * (CurrentWeapon->GetCriticalRate() + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_DAMAGE) );
			//return (1 + CurrentEnemyData->CriticalRate + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL_RATE) ) * (CurrentWeapon->GetCriticalRate() + CurStatInfo.GetStat(EBufStatType::BST_CRITICAL) );
	}
	return 0.f;
}

float UTPCharacterStatComponent::GetFinalPierceRate()
{
// 	if (IsPlayer)
// 		return CurrentStatData->Pierce;
// 	else
// 		return CurrentEnemyData->Pierce;
	return CurStatInfo.GetStat(EBufStatType::BST_PIERCE);
}

float UTPCharacterStatComponent::GetHPRatio()
{
	return (GetHPMax() < KINDA_SMALL_NUMBER) ? 0.f : (CurrentHP / GetHPMax());
}

float UTPCharacterStatComponent::GetStaminaRatio()
{
	return (GetStaminaMax() < KINDA_SMALL_NUMBER) ? 0.f : (CurrentStamina / GetStaminaMax());
}

float UTPCharacterStatComponent::GetShieldRatio()
{
	return (GetShieldMax() < KINDA_SMALL_NUMBER) ? 0.f : (CurrentShield / GetShieldMax());
}

int32 UTPCharacterStatComponent::GetDropExp()
{
	return CurStatInfo.GetStat(EBufStatType::BST_DROP_EXP);
}

float UTPCharacterStatComponent::GetReloadSpd()
{
	return CurStatInfo.GetStat(EBufStatType::BST_RELOAD_SPD);
}

float UTPCharacterStatComponent::GetFireRate()
{
	return CurStatInfo.GetStat(EBufStatType::BST_FIRE_RATE) * CurrentWeapon->GetFireSpd();
}


float UTPCharacterStatComponent::GetAccuracy()
{
	return CurStatInfo.GetStat(EBufStatType::BST_ACCURACY) * CurrentWeapon->GetAccuracy();
}

float UTPCharacterStatComponent::GetAccuracyRecovery()
{
	return CurrentWeapon->GetAccuracyRecover();
}

float UTPCharacterStatComponent::GetAccuracyDown()
{
	return CurrentWeapon->GetAccuracyDown();
}

float UTPCharacterStatComponent::GetAccuracyMin()
{
	return CurrentWeapon->GetAccuracyMin();
}

float UTPCharacterStatComponent::GetRecoil()
{
	float FinalRecoil = CurrentWeapon->GetRecoil() * CurStatInfo.GetStat(EBufStatType::BST_RECOIL);
	return FinalRecoil;
}

float UTPCharacterStatComponent::GetBulletSpd()
{
	return CurStatInfo.GetStat(EBufStatType::BST_BULLET_SPD) * CurrentWeapon->GetBulletSpd();
}

float UTPCharacterStatComponent::GetDefencePoint()
{
	if (IsPlayer)
		//  (방어력 + 추가 방어력) * (방어력 배율 + 추가 방어력 배율)
		return CurStatInfo.GetStat(EBufStatType::BST_DEFENCE_RATE) * CurStatInfo.GetStat(EBufStatType::BST_DEFENCE);
	else
		// (몬스터 방어력 + 추가 방어력)
		return CurStatInfo.GetStat(EBufStatType::BST_DEFENCE);
}

void UTPCharacterStatComponent::SetCurWeapon(TWeakObjectPtr<class ATPWeapon> InNewWeapon)
{
	CurrentWeapon = InNewWeapon;
}

void UTPCharacterStatComponent::SetIsHip(bool InIsHip)
{
	IsHip = InIsHip;

	CurrentAccuracyRate = MaxAccuracyRate = GetAccuracy();
	CurAccuracyRecovery = GetAccuracyRecovery();
	CurAccuracyDown = GetAccuracyDown();
	if (CurrentAccuracyRate > MaxAccuracyRate)
		CurrentAccuracyRate = MaxAccuracyRate;
	OnAccuracyChanged.Broadcast();
}

float UTPCharacterStatComponent::GetAccuracyRatio()
{
	return 1.f - CurrentAccuracyRate;
}

float UTPCharacterStatComponent::GetJumpForce()
{
	return CurStatInfo.GetStat(EBufStatType::BST_JUMP_FORCE);
}

float UTPCharacterStatComponent::GetMoveSpd()
{
	return CurStatInfo.GetStat(EBufStatType::BST_MOVE_SPD);
}

void UTPCharacterStatComponent::RecoverAccuracy(float DeltaTime)
{
	if (CurrentAccuracyRate >= MaxAccuracyRate)
		return;
	CurrentAccuracyRate += CurAccuracyRecovery * DeltaTime;
	if (CurrentAccuracyRate > MaxAccuracyRate)
		CurrentAccuracyRate = MaxAccuracyRate;
	TPLOG(Warning, TEXT("Accuarcyy #Recovery# : %f"), CurrentAccuracyRate);
	OnAccuracyChanged.Broadcast();
}

void UTPCharacterStatComponent::DecreaseAccuracy()
{
	CurrentAccuracyRate -= CurAccuracyDown;
	if (CurrentAccuracyRate <= GetAccuracyMin())
		CurrentAccuracyRate = GetAccuracyMin();
	TPLOG(Warning, TEXT("Accuarcyy <Decrease> : %f"), CurrentAccuracyRate);
	OnAccuracyChanged.Broadcast();
}

