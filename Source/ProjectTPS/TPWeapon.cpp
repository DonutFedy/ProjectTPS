// Fill out your copyright notice in the Description page of Project Settings.


#include "TPWeapon.h"
#include "TPCharacter.h"
#include "TPGameInstance.h"
#include "Table/TPBulletRecoilData.h"
#include "Table/TPWeaponTable.h"
#include "Engine/EngineTypes.h"



// Sets default values
ATPWeapon::ATPWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	IsDrop = false;
	WeaponIndex = 6;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("WEAPON_GETTER"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	Trigger->SetupAttachment(RootComponent);
	Trigger->SetBoxExtent(FVector(40.f, 42.f, 30.f));
	Trigger->SetCollisionProfileName(TEXT("GetWeapon"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WAEPON(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapon/Pistol/Mesh/SK_Pistol.SK_Pistol'"));
	if (SK_WAEPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WAEPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	AmmoRemain = 0;
	AttackRange = 0;
	AmmoCharge = 0;
	AmmoChargeCurrent = 0;
}

// Called when the game starts or when spawned
void ATPWeapon::BeginPlay()
{
	Super::BeginPlay();

// 	AmmoRemain = FMath::RandRange(AmmoMin, AmmoMax);
// 	AmmoCharge = FMath::RandRange(AmmoChargeMin, AmmoChargeMax);
	SetGetterTrigger(IsDrop == true);


	// using WeaponIndex
}

void ATPWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATPWeapon::OnCharacterOverlap);
}

float ATPWeapon::GetAmmo() const
{
	return AmmoChargeCurrent;
}

float ATPWeapon::GetAmmoRemain() const
{
	return AmmoRemain;
}

bool ATPWeapon::CheckShotBullet()
{
	if (AmmoChargeCurrent > 0 || AmmoRemain > 0)
		return true;
	return false;
}

bool ATPWeapon::NeedCharge()
{
	switch (AnimAttackType)
	{
	case EAnimAttackType::PISTOL:
	case EAnimAttackType::RIFLE:
	{
		if (AmmoRemain > 0)
		{
			if (AmmoChargeCurrent <= 0)
			{
				return true;
			}
		}
	}
		break;
	default:
		break;
	}
	return false;
}

bool ATPWeapon::CheckCanCharge()
{
	switch (AnimAttackType)
	{
	case EAnimAttackType::PISTOL:
	case EAnimAttackType::RIFLE:
	{
		if (AmmoRemain > 0)
		{
			if (AmmoChargeCurrent < AmmoCharge)
			{
				return true;
			}
		}
	}
	break;
	default:
		break;
	}
	return false;
}

bool ATPWeapon::TryShotBullet()
{
	if (AmmoChargeCurrent > 0)
	{
		--AmmoChargeCurrent;
	}
// 	else if (AmmoRemain > 0)
// 	{
// 		if (AmmoRemain > AmmoCharge)
// 		{
// 			AmmoRemain -= AmmoCharge;
// 			AmmoChargeCurrent = AmmoCharge;
// 		}
// 		else
// 		{
// 			AmmoChargeCurrent = AmmoRemain;
// 			AmmoRemain = 0;
// 		}
// 	}
	else
	{
		return false;
	}
	OnWeaponStateChanged.Broadcast(AmmoChargeCurrent, AmmoRemain);

	return true;
}

bool ATPWeapon::Reload()
{
	if (AmmoRemain <= 0)
		return false;


	int32 NeedAmmoCount = 0;
	if (AmmoChargeCurrent > 0)
	{
		NeedAmmoCount = AmmoCharge - AmmoChargeCurrent;
	}
	else
	{
		NeedAmmoCount = AmmoCharge;
	}
	int32 ResultNeedAmmoCount = NeedAmmoCount;
	if (bIsPlayer && AmmoRemain < NeedAmmoCount)
	{
		ResultNeedAmmoCount = AmmoRemain;
	}

	if (ResultNeedAmmoCount > 0)
	{
		AmmoRemain -= ResultNeedAmmoCount;
		if(bIsPlayer ==false && AmmoRemain <= 0)
			AmmoRemain = 1;
		AmmoChargeCurrent += ResultNeedAmmoCount;
	}

	OnWeaponStateChanged.Broadcast(AmmoChargeCurrent, AmmoRemain);
	return true;
}

float ATPWeapon::GetAttackRange()
{
	
	return AttackRange;
}

float ATPWeapon::GetAttackDamage() const
{
	return Attack;
}

float ATPWeapon::GetCriticalRate() const
{
	return CriticalRate;
}

float ATPWeapon::GetCriticalDamage() const
{
	return CriticalDamage;
}

float ATPWeapon::GetAccuracy()
{
	if(IsHip)
		return HipAccuracy;
	else
		return AdsAccuracy;
}

float ATPWeapon::GetAccuracyDown()
{
	if (IsHip)
		return HipAccuracyDown;
	else
		return AdsAccuracyDown;
}

float ATPWeapon::GetAccuracyRecover()
{
	return AccuracyRecover;
}

float ATPWeapon::GetAccuracyMin()
{
	return MinAccuracy;
}

float ATPWeapon::GetRecoil()
{
	return Recoil;
}

float ATPWeapon::GetFireSpd()
{
	return FireRate;
}

float ATPWeapon::GetBulletSpd()
{
	return BulletSpd;
}

float ATPWeapon::GetAdsLerpTime()
{
	return AdsTime;
}

void ATPWeapon::SetWeapon(bool InbIsPlayer)
{
	IsHip = true;
	bIsPlayer = InbIsPlayer;

	auto ABGameInstance = Cast<UTPGameInstance>(GetGameInstance());
	FTPWeaponTable* WeaponData = ABGameInstance->GetTPWeaponData(WeaponIndex);
	if (WeaponData)
	{
		//Name = WeaponData->Name;
		Rarity = WeaponData->Rarity;
		Attack = WeaponData->Attack;
		BulletSpd = WeaponData->BulletSpd;
		AdsTime = WeaponData->AdsTime;
		FireRate = WeaponData->FireRate;
		HipAccuracy = WeaponData->HipAccuracy;
		HipAccuracyDown = WeaponData->HipAccuracyDown;
		AdsAccuracy = WeaponData->AdsAccuracy;
		AdsAccuracyDown = WeaponData->AdsAccuracyDown;
		AccuracyRecover = WeaponData->AccuracyRecover;
		MinAccuracy = WeaponData->MinAccuracy;
		ChangeTime = WeaponData->ChangeTime;
		ReloadTime = WeaponData->ReloadTime;
		Pierce = WeaponData->Pierce;
		Recoil = WeaponData->Recoil;
		CriticalRate = WeaponData->CriticalRate;
		CriticalDamage = WeaponData->CriticalDamage;

		AmmoRemain = WeaponData->StartAmmo;
		AmmoCharge = WeaponData->Magazine;
	}
	CurrentRecoilIndex = 0;
	ArrRecoil.Empty();
	FTPBulletRecoilData* RecoilData = ABGameInstance->GetTPRecilData(WeaponIndex);
	if (RecoilData)
	{
		ArrRecoil = RecoilData->Recoil;
	}
}

FVector2D ATPWeapon::GetCurRecoil()
{
	if (ArrRecoil.Num() > CurrentRecoilIndex)
	{
		int32 CurIndex = CurrentRecoilIndex;
		++CurrentRecoilIndex;
		if (ArrRecoil.Num() <= CurrentRecoilIndex)
			CurrentRecoilIndex = 0;
		return ArrRecoil[CurIndex];
	}

	return FVector2D::Zero();
}

void ATPWeapon::SetResetFireCombo()
{
	CurrentRecoilIndex = 0;
}


void ATPWeapon::SetHipMode(bool IsNextStateHip)
{
	IsHip = IsNextStateHip;
}

void ATPWeapon::SetGetterTrigger(bool InAvailable)
{
	ECollisionEnabled::Type CollType = ECollisionEnabled::NoCollision;
	if(InAvailable)
		CollType = ECollisionEnabled::QueryOnly;
	Trigger->SetCollisionEnabled(CollType);
}

void ATPWeapon::GetBulletSpawnPosNRot(FVector& outVec, FRotator& outRot)
{
	outVec = Weapon->GetSocketLocation("Muzzle");
	outRot = Weapon->GetSocketRotation("Muzzle");
}


// Called every frame
void ATPWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATPWeapon::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TPLOG_S(Warning);

	auto TPCharacter = Cast<ATPCharacter>(OtherActor);
	TPCHECK(TPCharacter != nullptr);

	if (TPCharacter != nullptr )
	{
		if (TPCharacter->CanSetWeapon())
		{
			TPCharacter->SetWeapon(this);
			Trigger->SetHiddenInGame(true, true);
			Trigger->SetCollisionProfileName(TEXT("NoCollision"));
			SetActorEnableCollision(false);
			SetGetterTrigger(false);
			OnWeaponStateChanged.Broadcast(AmmoChargeCurrent, AmmoRemain);
		}
		else
		{
			TPLOG(Warning, TEXT("%s Can't equip Weapon currently."), *TPCharacter->GetName());
		}
	}
}
