// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "GameFramework/Character.h"
#include "TPCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class PROJECTTPS_API ATPCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ATPCharacter();
	//  [3/21/2025 ehgns] 상태값
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;
	//  [3/21/2025 ehgns] 상태값 end

	int32 GetExp() const;
	float GetFinalAttackRange() const;
	float GetFinalAttackAngle() const;
	float GetFinalAttackDamage() const;
	float GetFinalCriticalDamageRate() const;
	float GetFinalCriticalRate() const;
	float GetFinalPierceRate() const;
	float GetFinalDefencePoint() const;

	void ResetParams();

	void InitCharacter();
	void PlayStage();


	void AddSkill(struct FTPSkillInitData& InAddSkillInfo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	enum class EControlMode
	{
		TPS,
		NPC,
	};

	void SetControlMode(EControlMode ControlMode);
	EControlMode CurrentControlMode = EControlMode::TPS;
	FVector DirectionToMove = FVector::ZeroVector;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EvnetInstigator, AActor* DamageCauser) override;
	virtual void OnDestroyed();


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//  [3/25/2025 ehgns] Weapon
	void ReloadAmmo();
	bool CanSetWeapon();
	void SetWeapon(class ATPWeapon* NewWeapon);
	UPROPERTY(VisibleAnywhere, Category=Weapon)
	class ATPWeapon* CurrentWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon)
	TSubclassOf<ATPWeapon> DefaultWeapon;
	float FireAnimSpd;
	float BulletSpd;
	//  [3/25/2025 ehgns] Weapon end


	// Ads
	void ChangeAdsMode();
	void SetAds(bool AdsMode);
	void LerpAds(float DeltaTime);

	enum class EAdsModeState
	{
		EAMS_HIP,
		EAMS_HIP_TO_ADS,
		EAMS_ADS_TO_HIP,
		EAMS_ADS,
	};

	EAdsModeState CurrentAdsModeState;
	float TargetArmLength;
	float PrevArmLength;
	FVector CurRelativeLocLerpAds;
	FVector RelativeLocLerpAds;
	float AdsLerpProgress;
	bool NeedLerpAds;
	float AdsLerpTime;
	// Ads end


	// recoil
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Recoil, Meta = (AllowPrivateAccess = true))
	bool UseRecoil = true;
	void ComboAttackRelease();
	// recoil end
	
	//Accuracy
public:
	void RecoverAccuracy(float DeltaTime);
	float GetBulletSplatterAngle();
private:
	void DecreaseAccuracy();

public:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Accuracy, Meta = (AllowPrivateAccess = true))
	float SplatterAngle = 30.f;
	// Hip / Ads 
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Accuracy, Meta = (AllowPrivateAccess = true))
	float RecoilLerpTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Accuracy, Meta = (AllowPrivateAccess = true))
	
	bool NeedRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Accuracy, Meta = (AllowPrivateAccess = true))
	
	FVector CurRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Accuracy, Meta = (AllowPrivateAccess = true))
	
	FVector TargetRecoil;
	void LerpRecoil(float DeltaTime);
public:
	void SetHipMode(bool IsNextStateHip);
	
	// Hip / Ads end
	//Accuracy end


// 	UPROPERTY(VisibleAnywhere, Category=Weapon)
// 	USkeletalMeshComponent* Weapon;
#pragma region Skill Component
	UPROPERTY(VisibleAnywhere, Category=Skill)
	TObjectPtr<class UTPSkillComponent> SkillComponent;
#pragma endregion


#pragma region Stat Component
	UPROPERTY(VisibleAnywhere, Category=Stat)
	TObjectPtr<class UTPCharacterStatComponent> CharacterStat;
#pragma endregion


	FOnNoParamDeletage OnDead;

	void RestartLevel();


	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;


private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Dash();
	void DashRelease();
	void ReloadFinish();
	virtual void Jump() override; 

	void ViewChange();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void ShotBullet();

	void OnAssetLoadCompleted();

private:


	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

	
//============================Anim==========================

protected:
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowprivateAccess = true))
// 	class USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowprivateAccess = true))
	TSubclassOf<class UTPAnimInstance> SetupABAnim;

	UPROPERTY()
	class UTPAnimInstance* ABAnim;

	//  [3/27/2025 ehgns] AI Play/Stop
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowprivateAccess=true))
	float DeadTimer;
	FTimerHandle DeadTimerHandle = {};
	
	// Hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowprivateAccess=true))
	float HitReactTimer;
	FTimerHandle HitReactTimerHandle = {};

	void AdjustWeaponHeight();
	FVector GetBulletDirection(FVector& BulletPos);

public:
	
//============================Anim End==========================
	
//============================Camera==========================
protected:
	// Camera
	float ArmLengthTo = 0.f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.f;
	float ArmRotationSpeed = 0.f;
public:
	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* SpringArm;

	FRotator TestPitch;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent* Camera;
//============================Camera end==========================

#pragma region Attack
//============================ATTACK==========================
protected:
public:
	void InputAttack();
	bool Attack();
	void AttackRelease();
	FOnAttackEndDelegate OnAttackEnd;


protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess = true))
	bool IsReloading;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess = true))
	float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackAngle;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;
	
	//  [3/25/2025 ehgns] 콤보 관련
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess = true))
	bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess = true))
	bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess = true))
	bool IsComboFire;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Attack, Meta=(AllowPrivateAccess = true))
	int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;
	//  [3/25/2025 ehgns] 콤보 관련 end
public:

//============================ATTACK end==========================
#pragma endregion

	
#pragma region Control
//============================Control==========================
protected:

public:
	int32 AssetIndex = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category=State, Meta=(AllowPrivateAccess = true))
	ECharacterState CurrentState;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category=State, Meta=(AllowPrivateAccess = true))
	bool bIsPlayer;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool IsDashNow;
	UPROPERTY()
	class ATPAIController* TPAIController;
	UPROPERTY()
	class ATPPlayerController* TPPlayerController;


	void SetTag(FString InTag);
	
//============================Control end==========================
#pragma endregion
};
