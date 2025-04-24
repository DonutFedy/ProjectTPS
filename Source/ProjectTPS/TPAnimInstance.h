// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "Animation/AnimInstance.h"
#include "TPAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBulletShootDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAnimNotifyDelegate);

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:



	UTPAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetPawnSpeed(float NewPawnSpd) { CurrentPawnSpeed = NewPawnSpd; }

	void SetAttackType(EAnimAttackType NewAttackType);
	void PlayAttackMontage(float PlaySPD = 1.f);
	void PlayReloadMontage(float PlaySPD = 1.f);
	void JumpToAttackMontageSection(int32 NewSection);

	void SetTakenDamage(bool InTakenDamage);
	void SetWeaponRotate(FVector NewRot);
public:
	FOnNextAttackCheckDelegate	OnNextAttackCheck;
	FOnBulletShootDelegate		OnBulletShoot;
	FOnAnimNotifyDelegate		OnDashEndCheck;
	FOnAnimNotifyDelegate		OnReloadEndCheck;
	void SetDeadAnim() { IsDead = true; }

	void SetDash(bool InIsDash){ IsDash  = InIsDash;}
	void SetReload(bool InIsReloading){ IsReloading = InIsReloading;}

	float GetAttackAnimLength();

private:
	UFUNCTION()
	void AnimNotify_Shoot();

	UFUNCTION()
	void AnimNotify_DashEnd();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_ReloadEnd();

	FName GetAttackMontageSectionName(int32 Section);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsReloading;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsHitReaction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FVector WeaponRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float WeaponRotPitch;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Attack, Meta = (AllowPrivateAccess=true))
	EAnimAttackType CurrentAttackType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Attack, Meta = (AllowPrivateAccess=true))
	UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ReloadMontage;

	
};
