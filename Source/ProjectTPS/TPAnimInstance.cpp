// Fill out your copyright notice in the Description page of Project Settings.


#include "TPAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UTPAnimInstance::UTPAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsInAir = false;
	IsDead = false;
	IsDash = false;
	CurrentAttackType = EAnimAttackType::NONE;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Weapon/Pistol/Animations/AM_MM_Pistol_Fire.AM_MM_Pistol_Fire'"));
	
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UTPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn))return;


	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}


void UTPAnimInstance::SetAttackType(EAnimAttackType NewAttackType)
{
	if( CurrentAttackType == NewAttackType )
		return;
	CurrentAttackType = NewAttackType;
	FString AttackMontagePath = "";
	FString ReloadMontagePath = "";
	switch (NewAttackType)
	{
	case EAnimAttackType::NEAR_ATTACK:
	{
		AttackMontagePath = "/Script/Engine.AnimMontage'/Game/Characters/Motage/AM_AIAttack.AM_AIAttack'";
		//ReloadMontagePath = "/Script/Engine.AnimMontage'/Game/Characters/Motage/AM_AIAttack.AM_AIAttack'";
	}
		break;
	case EAnimAttackType::PISTOL:
	{
		AttackMontagePath = "/Script/Engine.AnimMontage'/Game/Weapon/Pistol/Animations/AM_MM_Pistol_Fire.AM_MM_Pistol_Fire'";
		ReloadMontagePath = "/Script/Engine.AnimMontage'/Game/Weapon/Pistol/Animations/AM_MM_Pistol_Reload.AM_MM_Pistol_Reload'";
	}
	break;

	default:
		break;
	}
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(*AttackMontagePath);
	//if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = LoadObject<UAnimMontage>(nullptr, *AttackMontagePath);
		ReloadMontage = nullptr;
		if(ReloadMontagePath.Len() > 0)
			ReloadMontage = LoadObject<UAnimMontage>(nullptr, *ReloadMontagePath);
	}
}

void UTPAnimInstance::PlayAttackMontage(float PlaySPD)
{
	TPCHECK(!IsDead);
	Montage_Stop(0.1f);
	float MontageDuration = Montage_Play(AttackMontage, PlaySPD);
	if (MontageDuration <= 0)
	{
		int t= 1;
	}
}

void UTPAnimInstance::PlayReloadMontage(float PlaySPD /*= 1.f*/)
{
	TPCHECK(!IsDead);
	Montage_Stop(0.1f);
	float MontageDuration = Montage_Play(ReloadMontage, PlaySPD);
	if (MontageDuration <= 0)
	{
		int t = 1;
	}
}

void UTPAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	TPCHECK(!IsDead);
	TPCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UTPAnimInstance::SetTakenDamage(bool InTakenDamage)
{
	bIsHitReaction = InTakenDamage;

}


void UTPAnimInstance::SetWeaponRotate(FVector NewRot)
{
	WeaponRot = NewRot;
	WeaponRotPitch = NewRot.X;
}

void UTPAnimInstance::AnimNotify_Shoot()
{
	OnBulletShoot.Broadcast();
}

void UTPAnimInstance::AnimNotify_DashEnd()
{
	OnDashEndCheck.Broadcast();
}

void UTPAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UTPAnimInstance::AnimNotify_ReloadEnd()
{
	OnReloadEndCheck.Broadcast();
}

FName UTPAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	TPCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

float UTPAnimInstance::GetAttackAnimLength()
{
	return AttackMontage->GetPlayLength() * 0.4f;
}

