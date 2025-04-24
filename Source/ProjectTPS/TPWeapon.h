// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "GameFramework/Actor.h"
#include "TPWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWaeponStateChangedDelegate, int32, int32);


UCLASS()
class PROJECTTPS_API ATPWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetWeapon(bool InbIsPlayer);


	EAnimAttackType GetAnimAttackType() {return AnimAttackType; }
	float GetAmmo() const;
	float GetAmmoRemain() const;
	bool CheckShotBullet();
	bool NeedCharge();
	bool CheckCanCharge();
	bool TryShotBullet();
	bool Reload();

	// 근접공격에 쓰임
	float GetAttackRange();

	float GetAttackDamage() const;
	float GetCriticalRate() const;
	float GetCriticalDamage() const;
	
	// Stat
	float GetAccuracy();
	float GetAccuracyDown();
	float GetAccuracyRecover();
	float GetAccuracyMin();
	float GetRecoil();
	float GetFireSpd();
	float GetBulletSpd();
	float GetAdsLerpTime();


	// Stat


	// recoil
public:
	FVector2D GetCurRecoil();
	void SetResetFireCombo();

private:
	TArray<FVector2D> ArrRecoil;
	int32 CurrentRecoilIndex;
	float FireAnimSpd;
	// recoil end

	// Hip / Ads 
public:
	void SetHipMode(bool IsNextStateHip);
	// Hip / Ads end


public:
	void SetGetterTrigger(bool InAvailable);

	void GetBulletSpawnPosNRot(FVector& outVec, FRotator& outRot);

	FOnWaeponStateChangedDelegate OnWeaponStateChanged;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category=Weapon)
	USkeletalMeshComponent* Weapon;
	
	UPROPERTY(VisibleAnywhere, Category=Weapon)
	UBoxComponent* Trigger;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	bool bIsPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	int32 WeaponIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	bool IsHip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	bool IsDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attack)
	EAnimAttackType AnimAttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AmmoRemain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AmmoCharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AmmoChargeCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackRange;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	int		Rarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	int		Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	BulletSpd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	AdsTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	HipAccuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	HipAccuracyDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	AdsAccuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	AdsAccuracyDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	AccuracyRecover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	MinAccuracy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	ChangeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	Pierce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	Recoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	CriticalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float	CriticalDamage;
};
