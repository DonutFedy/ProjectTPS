// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCharacter.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "TPGameInstance.h"
#include "TPGameMode.h"
#include "TPAnimInstance.h"
#include "TPWeapon.h"
#include "TPCharacterStatComponent.h"
#include "TPSkillComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Components/WidgetComponent.h"
#include "TPCharacterHUD.h"
#include "TPAIController.h"
#include "TPCharacterSetting.h"
#include "TPPlayerController.h"
#include "Engine/StreamableManager.h"
#include "TPPlayerState.h"
#include "TPBullet.h"
#include "TPInGameUI.h"
#include "GameFramework/Controller.h"
#include "Table/TPBulletRecoilData.h"
#include "Management/TPStageManager.h"

// Sets default values
ATPCharacter::ATPCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UTPCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	SkillComponent = CreateDefaultSubobject<UTPSkillComponent>(TEXT("CHARACTERSKILL"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetCollisionProfileName(TEXT("TPCharacter"), true);
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	SpringArm->TargetArmLength = 100.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 30.f, 70.f));
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

// 	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHAR(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Heroes/Mannequin/Meshes/SKM_Quinn.SKM_Quinn'"));
// 	if (SK_CHAR.Succeeded())
// 	{
// 		GetMesh()->SetSkeletalMesh(SK_CHAR.Object);
// 	}
// 
// 	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
// 	static ConstructorHelpers::FClassFinder<UAnimInstance> TP_ANIM(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/AB_TPCharacter.AB_TPCharacter_C'"));
// 	if (TP_ANIM.Succeeded())
// 	{
// 		GetMesh()->SetAnimInstanceClass(TP_ANIM.Class);
// 	}


	ResetParams();
	SetControlMode(EControlMode::TPS);

	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TPCharacter"));


	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 210.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_HPBar.UI_HPBar_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}


	AIControllerClass = ATPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);


}

void ATPCharacter::SetCharacterState(ECharacterState NewState)
{
	TPCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		if (bIsPlayer)
		{
			DisableInput(TPPlayerController);
			TPPlayerController->GetHUDWidget()->BindCharacterState(CharacterStat);

			auto TPPlayerState = Cast<ATPPlayerState>(GetPlayerState());
			TPCHECK(TPPlayerState != nullptr);
			CharacterStat->SetStatComponent(TPPlayerState->GetCharacterLevel(), this);

			SetTag("Player");
		}
		else
		{
// 			auto ABGameMode = Cast<ATPGameMode>(GetWorld()->GetAuthGameMode());
// 			TPCHECK(ABGameMode != nullptr);
// 			int32 TargetLevel = FMath::CeilToInt((float)ABGameMode->GetScore() * 0.8f);
// 			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			//TPLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			//CharacterStat->SetNewLevel(FinalLevel);
			CharacterStat->SetAI(1, this);

			SetTag("Enemy");
		}

		HPBarWidget->SetHiddenInGame(true);
		SetActorHiddenInGame(true);
		SetCanBeDamaged(false);
	}
	break;
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		SetCanBeDamaged(true);

		CharacterStat->OnHPIsZero.AddLambda([this]()->void {
			SetCharacterState(ECharacterState::DEAD);
			OnDead.Broadcast();
			});


		GetCharacterMovement()->JumpZVelocity = CharacterStat->GetJumpForce();
		GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetMoveSpd();

		if (bIsPlayer)
		{
			ReloadFinish();

			SetControlMode(EControlMode::TPS);
			EnableInput(TPPlayerController);

			TArray<FTPSkillInitData> StartSkills;
			//  [4/24/2025 ehgns] Add Skill Test
			//Test
			FTPSkillInitData NewPassiveSkill;
			NewPassiveSkill.SkillIndex = 1002;
			NewPassiveSkill.SkillLv = 1;
			StartSkills.Add(NewPassiveSkill);

			NewPassiveSkill.SkillIndex = 1004;
			NewPassiveSkill.SkillLv = 1;
			StartSkills.Add(NewPassiveSkill);

			NewPassiveSkill.SkillIndex = 1008;
			NewPassiveSkill.SkillLv = 1;
			StartSkills.Add(NewPassiveSkill);
			//Test end
			// 초반 스킬 세팅
			SkillComponent->SetSkillComponent(CharacterStat, StartSkills);
		}
		else
		{
			HPBarWidget->SetHiddenInGame(false);
			SetControlMode(EControlMode::NPC);
			TPAIController->RunAI();

			auto CharacterWidget = Cast<UTPCharacterHUD>(HPBarWidget->GetUserWidgetObject());
			TPCHECK(CharacterWidget != nullptr);
			CharacterWidget->BindCharacterStat(CharacterStat);
		}
	}
	break;
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		ABAnim->SetDeadAnim();
		SetCanBeDamaged(false);


		if (bIsPlayer)
		{
			DisableInput(TPPlayerController);
		}
		else
		{
			TPAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
			if (bIsPlayer)
			{
				//TPPlayerController->RestartLevel();
			}
			else
			{
				
				//Destroy();



			}

			if(CurrentWeapon->IsValidLowLevel())
			{
				CurrentWeapon->SetActorHiddenInGame(true);
				CurrentWeapon->SetActorTickEnabled(false);
			}

			SetActorHiddenInGame(true);
			SetActorTickEnabled(false);
			}), DeadTimer, false);
	}
	break;
	default:
		break;
	}
}

ECharacterState ATPCharacter::GetCharacterState() const
{
	return CurrentState;
}

int32 ATPCharacter::GetExp() const
{
	return 0;
	//return CharacterStat->GetDropExp();
}

float ATPCharacter::GetFinalAttackRange() const
{
	return CharacterStat->GetAttackRange();
	//return AttackRange;
}

float ATPCharacter::GetFinalAttackAngle() const
{
	return AttackAngle;
}

float ATPCharacter::GetFinalAttackDamage() const
{
	return CharacterStat->GetAttack();
}

float ATPCharacter::GetFinalCriticalDamageRate() const
{
	//치명타 확률 계산 = (무기 치명타 확률 + 추가 치명타 확률) * (1+(캐릭터 치명타 확률 배율 + 추가 치명타 확률 배율))	
	return CharacterStat->GetFinalCriticalDamageRate();
}

float ATPCharacter::GetFinalCriticalRate() const
{
	//치명타 확률 계산 = (무기 치명타 확률 + 추가 치명타 확률) * (1+(캐릭터 치명타 확률 배율 + 추가 치명타 확률 배율))	
	return CharacterStat->GetFinalCriticalRate();
}

float ATPCharacter::GetFinalPierceRate() const
{
	return CharacterStat->GetFinalPierceRate();
}

float ATPCharacter::GetFinalDefencePoint() const
{
	return CharacterStat->GetDefencePoint();
}

void ATPCharacter::ResetParams()
{
	ArmLengthSpeed = 3.f;
	ArmRotationSpeed = 10.f;
	SetControlMode(EControlMode::TPS);

	GetCharacterMovement()->JumpZVelocity = 0.f;

	NeedRecoil = false;

	NeedLerpAds = false;

	MaxCombo = 4;
	AttackEndComboState();

	AttackRange = 800.f;
	AttackAngle = 5.f;
	AttackRadius = 50.f;

	AssetIndex = 3;

	DeadTimer = 5.f;
	HitReactTimer = 0.15f;

	IsDashNow = false;
}

void ATPCharacter::InitCharacter()
{
	ResetParams();


	// 1.
	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		TPPlayerController = Cast<ATPPlayerController>(GetController());
		TPCHECK(TPPlayerController != nullptr);
		TPPlayerController->InitController();
		DisableInput(TPPlayerController);
	}
	else
	{
		TPAIController = Cast<ATPAIController>(GetController());
		TPCHECK(TPAIController != nullptr);
	}

	auto DefaultSetting = GetDefault<UTPCharacterSetting>();

	if (bIsPlayer)
	{
		auto TPPlayerState = Cast<ATPPlayerState>(GetPlayerState());
		TPCHECK(TPPlayerState != nullptr);
		AssetIndex = TPPlayerState->GetCharacterIndex();
	}
	else
	{
		AssetIndex = FMath::RandRange(1, DefaultSetting->CharacterAssets.Num() - 1);
	}

	GetMesh()->SetAnimInstanceClass(SetupABAnim);
	GetMesh()->SetCollisionProfileName(TEXT("TPCharacter"), true);

	auto CharacterWidget = Cast<UTPCharacterHUD>(HPBarWidget->GetUserWidgetObject());
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

	// 3.
// 	{
//  		//CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
// 
// 		auto ABGameInstance = Cast<UTPGameInstance>(GetGameInstance());
// 		TPCHECK(ABGameInstance != nullptr);
// 		if (ABGameInstance != nullptr)
// 		{
// 			//AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &ATPCharacter::OnAssetLoadCompleted));
// 			SetCharacterState(ECharacterState::LOADING);
// 		}
// 	}

	ABAnim = Cast<UTPAnimInstance>(GetMesh()->GetAnimInstance());
	TPCHECK(ABAnim != nullptr);

	ABAnim->OnMontageEnded.AddDynamic(this, &ATPCharacter::OnAttackMontageEnded);

	ABAnim->OnNextAttackCheck.AddLambda([this]()-> void {
		TPLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			if (CurrentWeapon == nullptr || CharacterStat->GetIsReloading() == true || CurrentWeapon->CheckShotBullet() == false)
				return;
			if (CurrentWeapon->NeedCharge())
			{
				ReloadAmmo();
				return;
			}
			AttackStartComboState();
			ABAnim->PlayAttackMontage(FireAnimSpd);
			//ABAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});

	ABAnim->OnDashEndCheck.AddLambda([this]()-> void {
		TPLOG(Warning, TEXT("OnDashEndCheck"));
		DashRelease();
		});

	ABAnim->OnReloadEndCheck.AddLambda([this]()-> void {
		TPLOG(Warning, TEXT("OnReloadEndCheck"));
		ReloadFinish();
		});

	ABAnim->OnBulletShoot.AddUObject(this, &ATPCharacter::ShotBullet);

	SetCharacterState(ECharacterState::LOADING);
	auto ABGameInstance = Cast<UTPGameInstance>(GetGameInstance());
	TPCHECK(ABGameInstance != nullptr);
	if (ABGameInstance != nullptr)
	{
		ATPWeapon* StartWeapon = nullptr;
		if (DefaultWeapon == nullptr)
		{
			FString WeaponPath = "";
			if (bIsPlayer)
			{
				WeaponPath = "/Script/Engine.Blueprint'/Game/Weapon/BP/BP_Pistol.BP_Pistol_C'";
			}
			else
			{
				WeaponPath = "/Script/Engine.Blueprint'/Game/Weapon/BP/BP_PistolNear.BP_PistolNear_C'";
			}
			StartWeapon = GetWorld()->SpawnActor<ATPWeapon>(LoadClass<ATPWeapon>(nullptr, *WeaponPath));
			//AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(WeaponPath, FStreamableDelegate::CreateUObject(this, &ATPCharacter::OnAssetLoadCompleted));
		}
		else
		{
			StartWeapon = GetWorld()->SpawnActor<ATPWeapon>(DefaultWeapon.Get());
			//AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(DefaultWeapon.Get(), FStreamableDelegate::CreateUObject(this, &ATPCharacter::OnAssetLoadCompleted));
		}
		//auto CurWeapon = GetWorld()->SpawnActor<ATPWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
		if (StartWeapon != nullptr)
		{
			SetWeapon(StartWeapon);
		}
		CurRelativeLocLerpAds = FVector(0.f, 40.f, 80.f);
		PrevArmLength = 200.f;
		SetAds(true);

	}
}

void ATPCharacter::PlayStage()
{
	SetCharacterState(ECharacterState::READY);

	EnableInput(TPPlayerController);
}

void ATPCharacter::AddSkill(FTPSkillInitData& InAddSkillInfo)
{
	TPCHECK(SkillComponent);
	if(SkillComponent)
		SkillComponent->AddSkill(InAddSkillInfo);
}

// Called when the game starts or when spawned
void ATPCharacter::BeginPlay()
{
	Super::BeginPlay();


}

void ATPCharacter::SetControlMode(EControlMode ControlMode)
{
	CurrentControlMode = ControlMode;
	switch (CurrentControlMode)
	{
	case ATPCharacter::EControlMode::TPS:
	{
		//SpringArm->TargetArmLength = 800.f;
		//SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
		ArmLengthTo = 450.f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
// 		SpringArm->bInheritPitch = true;
// 		SpringArm->bInheritRoll = true;
// 		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}
	break;
	case EControlMode::NPC:
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	}
	break;
	default:
		break;
	}
}

// Called every frame
void ATPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentState < ECharacterState::READY)
		return;

	AdjustWeaponHeight();

	

	RecoverAccuracy(DeltaTime);
	LerpRecoil(DeltaTime);
	LerpAds(DeltaTime);
	CharacterStat->RecoveryShield(DeltaTime);
	CharacterStat->RecoveryHP(DeltaTime);
	if(bIsPlayer)
		CharacterStat->RecoveryStamina(DeltaTime);

	SkillComponent->TickSkillComponent(DeltaTime);
}

void ATPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

// 	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
// 		TPLOG(Warning, TEXT("OnHPIsZero"));
// 		ABAnim->SetDeadAnim();
// 		SetActorEnableCollision(false);
// 		});

}

float ATPCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	TPLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	// 	if (FinalDamage > 0.f)
	// 	{
	// 		ABAnim->SetDeadAnim();
	// 		SetActorEnableCollision(false);
	// 	}
	CharacterStat->SetDamage(FinalDamage);

	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto CurTPPlayerController = Cast<ATPPlayerController >(EventInstigator);
			TPCHECK(CurTPPlayerController != nullptr, 0.0f);
//			CurTPPlayerController->NPCKill(this);
		}
	}

	ABAnim->SetTakenDamage(true);
	GetWorld()->GetTimerManager().SetTimer(HitReactTimerHandle, FTimerDelegate::CreateLambda([this]()->void {
		ABAnim->SetTakenDamage(false);
		}), HitReactTimer, false); 

	return FinalDamage;
}

void ATPCharacter::OnDestroyed()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}
	CurrentWeapon = nullptr;
}

void ATPCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::TPS);
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

// Called to bind functionality to input
void ATPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ATPCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ATPCharacter::LeftRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &ATPCharacter::ViewChange);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &ATPCharacter::Dash);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Released, this, &ATPCharacter::DashRelease);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ATPCharacter::InputAttack);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &ATPCharacter::AttackRelease);
	PlayerInputComponent->BindAction(TEXT("Ads"), EInputEvent::IE_Pressed, this, &ATPCharacter::ChangeAdsMode);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &ATPCharacter::ReloadAmmo);
}

void ATPCharacter::ReloadAmmo()
{
	if (CurrentWeapon->CheckCanCharge() == false)
		return;
	AttackRelease();
	CharacterStat->SetIsReloading(true);
	ABAnim->SetReload(true);
	ABAnim->PlayReloadMontage(CharacterStat->GetReloadSpd());
}

bool ATPCharacter::CanSetWeapon()
{
	return true;
}

void ATPCharacter::SetWeapon(class ATPWeapon* NewWeapon)
{
	TPCHECK(NewWeapon != nullptr);
	if (CurrentWeapon == NewWeapon)
		return;
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->OnWeaponStateChanged.Clear();
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (NewWeapon != nullptr)
	{
		CurrentWeapon = NewWeapon;
		ABAnim->SetAttackType(NewWeapon->GetAnimAttackType());
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		NewWeapon->SetGetterTrigger(false);
		NewWeapon->SetWeapon(bIsPlayer);

		CharacterStat->SetCurWeapon(NewWeapon);

		// Set WeaponStat
		FireAnimSpd = CharacterStat->GetFireRate(); // 캐릭터 fireRate * WeaponFireRate
		BulletSpd = CharacterStat->GetBulletSpd() * 2000.f;
		AdsLerpTime = CurrentWeapon->GetAdsLerpTime();

		RecoilLerpTime = 1 / (ABAnim->GetAttackAnimLength() / FireAnimSpd);

		SetHipMode(true);

		// 연발 상태 체크
		IsComboFire = false;// 원래는 무기에서 받아와야함.
		if (FireAnimSpd <= KINDA_SMALL_NUMBER)
		{
			FireAnimSpd = 1.f;
			if (!bIsPlayer && FMath::RandRange(0, 100) < 60)
				IsComboFire = true;
		}
		else
			IsComboFire = true;
			

			

		if(bIsPlayer)
			NewWeapon->OnWeaponStateChanged.AddUObject(TPPlayerController->GetHUDWidget(), &UTPInGameUI::UpdateWaeponState);
	}

}


void ATPCharacter::ChangeAdsMode()
{
	bool NextMode = !CharacterStat->GetIsHip(); // NextMode == true > Hip , false -> Ads
	SetAds(NextMode);
}

void ATPCharacter::SetAds(bool AdsMode)
{
	bool NextMode = AdsMode;
	//CharacterStat->SetIsHip(AdsMode);

	AdsLerpProgress = 0.f;
	PrevArmLength = SpringArm->TargetArmLength;
	if (NextMode) // Hip
	{
		CurrentAdsModeState = EAdsModeState::EAMS_ADS_TO_HIP;
		RelativeLocLerpAds = FVector(0.f, 40.f, 80.f);
		TargetArmLength = 200.f;
	}
	else
	{
		CurrentAdsModeState = EAdsModeState::EAMS_HIP_TO_ADS;
		RelativeLocLerpAds = FVector(0.f, 60.f, 74.f);
		TargetArmLength = 72.f;
	}
	NeedLerpAds = true;
}

void ATPCharacter::LerpAds(float DeltaTime)
{

	if (NeedLerpAds == false)
	{
		return;
	}

	AdsLerpProgress += DeltaTime;
	float Alpha = FMath::Clamp(AdsLerpProgress / AdsLerpTime, 0.f, 1.f);
	float CurrentArmLength = FMath::Lerp(PrevArmLength, TargetArmLength, Alpha);
	SpringArm->TargetArmLength = CurrentArmLength;

	CurRelativeLocLerpAds = FMath::Lerp(CurRelativeLocLerpAds, RelativeLocLerpAds, Alpha);

	SpringArm->SetRelativeLocation(CurRelativeLocLerpAds);




	if (Alpha >= 1.0f)
	{
		NeedLerpAds = false;

		if (CharacterStat->GetIsHip() == false)
		{
			CurrentWeapon->SetHipMode(true);
			CharacterStat->SetIsHip(true);
			CurrentAdsModeState = EAdsModeState::EAMS_HIP;
			SpringArm->bInheritPitch = true;
			SpringArm->bInheritRoll = true;
			SpringArm->bInheritYaw = true;
			SpringArm->bDoCollisionTest = true;
		}
		else
		{
			CurrentWeapon->SetHipMode(false);
			CharacterStat->SetIsHip(false);
			CurrentAdsModeState = EAdsModeState::EAMS_ADS;
// 			SpringArm->bInheritPitch = false;
// 			SpringArm->bInheritRoll = false;
// 			SpringArm->bInheritYaw = false;
			SpringArm->bDoCollisionTest = false;
		}
	}
}

void ATPCharacter::ComboAttackRelease()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ATPCharacter::SetHipMode(bool IsNextStateHip)
{
	CharacterStat->SetIsHip(IsNextStateHip);
	CurrentWeapon->SetHipMode(IsNextStateHip);
}

void ATPCharacter::RestartLevel()
{
	TPPlayerController->RestartLevel();
}

void ATPCharacter::RecoverAccuracy(float DeltaTime)
{
	CharacterStat->RecoverAccuracy(DeltaTime);
}

float ATPCharacter::GetBulletSplatterAngle()
{
	//float AimSize = UiComponent->GetAimSize();
	// 원점기준 랜덤한 각도, 랜덤한 반지름으로 Offset값을 만들어준다.
	float RandAngle = FMath::FRandRange(0.f, (1.f - CharacterStat->GetCurrentAccuracyRate()) * SplatterAngle);
	DecreaseAccuracy();
	return RandAngle;
}

void ATPCharacter::DecreaseAccuracy()
{
	CharacterStat->DecreaseAccuracy();
}



void ATPCharacter::LerpRecoil(float DeltaTime)
{
	if(	NeedRecoil == false )
		return;
	FVector UpdateRecoil = FMath::VInterpTo(CurRecoil,TargetRecoil, DeltaTime, RecoilLerpTime);
	FVector DeltaRecoil = UpdateRecoil - CurRecoil;
	CurRecoil = UpdateRecoil;

	AddControllerPitchInput(-DeltaRecoil.Y);
	AddControllerYawInput(-DeltaRecoil.X);

	if ((TargetRecoil - CurRecoil).Size() < KINDA_SMALL_NUMBER)
	{
		NeedRecoil = false;
	}
}

void ATPCharacter::UpDown(float NewAxisValue)
{
	if (GetCharacterState() == ECharacterState::DEAD)
		return;
	switch (CurrentControlMode)
	{
	case ATPCharacter::EControlMode::TPS:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	default:
		break;
	}
}
void ATPCharacter::LeftRight(float NewAxisValue)
{
	if (GetCharacterState() == ECharacterState::DEAD)
		return;
	switch (CurrentControlMode)
	{
	case ATPCharacter::EControlMode::TPS:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	default:
		break;
	}
}
void ATPCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case ATPCharacter::EControlMode::TPS:
	{
// 		TestPitch.Pitch += NewAxisValue;
// 		SpringArm->AddLocalRotation(TestPitch);
		AddControllerPitchInput(NewAxisValue);
		//SetActorRotation(GetControlRotation());
	}
		break;
	default:
		break;
	}
}
void ATPCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case ATPCharacter::EControlMode::TPS:
	{
		AddControllerYawInput(NewAxisValue);
		SetActorRotation(GetControlRotation());
	}
		break;
	default:
		break;
	}
}

void ATPCharacter::Dash()
{
	if(IsDashNow)
		return;
	if (GetMovementComponent()->IsFalling())
		return;

	if(CharacterStat->UseStamina(10) == false)
		return;

	ABAnim->SetDash(true);
	SetCanBeDamaged(false);

	if(bIsPlayer)
		DisableInput(TPPlayerController);

	GetCharacterMovement()->GroundFriction = 0.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;

	FVector DashDirection = GetVelocity().GetSafeNormal();
	if(DashDirection.IsZero())
		DashDirection = GetActorForwardVector();
	float DashSpeed = 2000.0f;  // 대시 속도 조절 가능

	LaunchCharacter(DashDirection * DashSpeed, true, true);
}

void ATPCharacter::DashRelease()
{
	ABAnim->SetDash(false);
	SetCanBeDamaged(true);

	GetCharacterMovement()->GroundFriction = 8.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;

	if (bIsPlayer)
		EnableInput(TPPlayerController);
}

void ATPCharacter::ReloadFinish()
{
	CharacterStat->SetIsReloading(false);
	ABAnim->SetReload(false);
	CurrentWeapon->Reload();
}

void ATPCharacter::Jump()
{
	Super::Jump();
	AttackRelease();
}

void ATPCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::TPS:
	{
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::TPS);
	}
	break;
	}

}

void ATPCharacter::AdjustWeaponHeight()
{
	if (!CurrentWeapon || !Camera) return;

	float CameraPitch = Camera->GetComponentRotation().Pitch;

	float Offset = 0.15f;
	if(CharacterStat->GetIsHip() == false)
		Offset = 0.17f;
	float PitchOffset = FMath::Clamp(CameraPitch * Offset, -60.0f, 60.0f);



	FQuat boneRot = GetMesh()->GetBoneQuaternion("spine_01");
	FVector CurBoneRot = boneRot.ToRotationVector();
	CurBoneRot.X = PitchOffset;
	ABAnim->SetWeaponRotate(CurBoneRot);
}

FVector ATPCharacter::GetBulletDirection(FVector& BulletPos)
{
	// 3.
	if (!TPPlayerController) return FVector::Zero();

// 	FVector ShootDirection; // 뷰포트 중앙 방향
// 
// 	int32 ViewportX, ViewportY;
// 	TPPlayerController->GetViewportSize(ViewportX, ViewportY); // 현재 뷰포트 크기 가져오기
// 
// 	FVector WorldLocation;
// 	TPPlayerController->DeprojectScreenPositionToWorld(ViewportX / 2.0f, ViewportY / 2.0f, WorldLocation, ShootDirection);
// 
// 	FVector VecTest = (BulletPos + ShootDirection * 5000.f);
// 	DrawDebugLine(GetWorld(), BulletPos, VecTest, FColor::Red, true, 1.f);
// 
// 	FVector ResultPoint = FMath::VRandCone(ShootDirection, GetBulletSplatterAngle());
// 	FVector ResultTarget = (BulletPos + ResultPoint * 5000.f);
// 	DrawDebugLine(GetWorld(), BulletPos, ResultTarget, FColor::Blue, true, 1.f);


	// 화면 중심 좌표 계산
	int32 ViewportX, ViewportY;
	TPPlayerController->GetViewportSize(ViewportX, ViewportY); // 현재 뷰포트 크기 가져오기
	FVector2D ScreenCenter(ViewportX / 2.f, ViewportY / 2.f);

	// 화면 중심에서 월드 방향 가져오기
	FVector WorldLocation, WorldDirection;
	TPPlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);


	FVector BulletStart = WorldLocation;
	//FVector BulletTargetPos = BulletStart + (WorldDirection * 10000); // Ray 길이
	float AccuacyRatio = CharacterStat->GetAccuracyRatio();
	float CurAimSize = TPPlayerController->GetHUDWidget()->GetAimImgSize() * AccuacyRatio;
	float SpreadScreenRatio = CurAimSize / ScreenCenter.X;


	float CameraFOV = Camera->FieldOfView; // or Camera->FieldOfView
	float HalfFOVRadians = FMath::DegreesToRadians(CameraFOV * 0.5f);
	float AngleRadians = SpreadScreenRatio * HalfFOVRadians;

 	// 탄튐
	DecreaseAccuracy();
	TPLOG(Warning, TEXT("Radians : %.1f"), AngleRadians);
	FVector BulletTargetPos = BulletStart + FMath::VRandCone(WorldDirection.GetSafeNormal(), AngleRadians) * 10000;
	GEngine->AddOnScreenDebugMessage(
		0,                  // 키: -1이면 새 메시지로 추가
		5.0f,                // 표시 시간 (초)
		FColor::Red,         // 색상
		BulletTargetPos.ToString() // 출력할 텍스트
	);
	GEngine->AddOnScreenDebugMessage(
		1,                  // 키: -1이면 새 메시지로 추가
		5.0f,                // 표시 시간 (초)
		FColor::Red,         // 색상
		FString::Printf(TEXT("Angle : %.1f"), AngleRadians) // 출력할 텍스트
	);


	//FVector BulletTargetPos = BulletStart + (WorldDirection + RandomOffset) * 10000 ;



	// Ray 설정
	FHitResult HitResult;
	FVector HitLocation;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 혹은 특정 Actor

	if (GetWorld()->LineTraceSingleByChannel(HitResult, BulletStart, BulletTargetPos, ECC_Visibility, Params))
	{
		HitLocation = HitResult.Location; // 충돌한 위치
	}
	else
	{
		HitLocation = BulletTargetPos; // 충돌하지 않으면 Ray 끝점
	}

	// ------------------------- DEUBG
// 	FVector TestTargetPos = BulletPos + BulletTargetPos * 10000;
// 	FHitResult HitResult;
// 	FVector HitLocation;
// 	if (GetWorld()->LineTraceSingleByChannel(HitResult, BulletPos, TestTargetPos, ECC_Visibility))
// 	{
// 		HitLocation = HitResult.Location; // 충돌한 위치
// 	}
// 	else
// 	{
// 		HitLocation = TestTargetPos; // 충돌하지 않으면 Ray 끝점
// 	}


	//  [4/17/2025 ehgns]
	DrawDebugCircle(GetWorld(), HitLocation, 10, 20, FColor::Red, false, 1);
	DrawDebugLine(GetWorld(), BulletPos, HitLocation, FColor::Red, false, 1.f);
	// ------------------------- DEUBG

	// 발사 방향 계산
	FVector ShootDirection = (HitLocation - BulletPos).GetSafeNormal();


	return ShootDirection;
	

// 	FVector Dir = (ShootDirection - BulletPos);
// 	Dir.Normalize();
// 	return Dir.Rotation(); // 방향을 회전 값으로 변환

	//2.
// 	APlayerController* PlayerController = Cast<APlayerController>(GetController());
// 	if (!PlayerController) return FRotator::ZeroRotator;
// 
// 	FVector CameraLocation;
// 	FRotator CameraRotation;
// 
// 	// 카메라 위치 & 회전값 가져오기
// 	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);


	// 1.
	// 라인 트레이스 시작 지점: 카메라 위치
// 	FVector Start = CameraLocation;
// 
// 	// 라인 트레이스 끝 지점: 화면 중앙에서 5000 단위 거리
// 	FVector End = Start + CameraRotation.Vector() * 5000.0f;
// 
// 	FHitResult HitResult;
// 	FCollisionQueryParams Params;
// 	Params.AddIgnoredActor(this); // 자기 자신 제외
// 
// 	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
// 	{
// 		// 맞은 지점 반환
// 		return (HitResult.ImpactPoint - CameraLocation).GetSafeNormal();
// 	}

	// 아무것도 맞지 않았다면 정면 방향 반환
	//return CameraRotation;
}

void ATPCharacter::InputAttack()
{
	Attack();
}

bool ATPCharacter::Attack()
{
	if (CurrentWeapon == nullptr || CharacterStat->GetIsReloading() == true || CurrentWeapon->CheckShotBullet() == false)
		return false;


	if (CurrentWeapon->NeedCharge())
	{
		ReloadAmmo();
		return false;
	}

	if (CharacterStat->GetIsAttacking())
	{
		TPCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo), false);
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		if(IsComboFire)
			IsComboInputOn = true;
		TPCHECK(CurrentCombo == 0, false);
		AttackStartComboState();
		ABAnim->PlayAttackMontage(FireAnimSpd);
		//ABAnim->JumpToAttackMontageSection(CurrentCombo);
		CharacterStat->SetIsAttacking(true);
	}
	return true;
}

void ATPCharacter::AttackRelease()
{
	if (IsComboFire)
	{
		ComboAttackRelease();
	}
}

void ATPCharacter::SetTag(FString InTag)
{
	GetCapsuleComponent()->ComponentTags.Empty();
	GetCapsuleComponent()->ComponentTags.Add(*InTag);
	Tags.Empty();
	Tags.Add(*InTag);
}

void ATPCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	TPLOG(Warning, TEXT("OnAttackMontageEnded"));
	TPCHECK(CharacterStat->GetIsAttacking());
	CharacterStat->SetIsAttacking(false);
	AttackEndComboState();
	OnAttackEnd.Broadcast();

	CurrentWeapon->SetResetFireCombo();
}

void ATPCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	if (IsComboFire == false)
		IsComboInputOn = false;
	TPCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);

}

void ATPCharacter::AttackEndComboState()
{
	if (IsComboFire )
		return;
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}


void ATPCharacter::ShotBullet()
{
	if (CurrentWeapon == nullptr )
		return;

	if (CurrentWeapon->GetAnimAttackType() == EAnimAttackType::NEAR_ATTACK)
	{
	// 근접공격용
		//FHitResult HitResult;
		TArray<FHitResult> HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);
		//bool bResult = GetWorld()->SweepSingleByChannel(
		bool bResult = GetWorld()->SweepMultiByChannel(
			HitResult,
			GetActorLocation(),
			GetActorLocation() + GetActorForwardVector() * GetFinalAttackRange(),
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel1,
			FCollisionShape::MakeSphere(AttackRadius),
			Params);

#if ENABLE_DRAW_DEBUG
// 		FVector TraceVec = GetActorForwardVector() * GetFinalAttackRange();
// 		FVector Center = GetActorLocation() + TraceVec * 0.5f;
// 		float HalfHeight = GetFinalAttackRange() * 0.5f + AttackRadius;
// 		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
// 		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
// 		float DebugLitTime = 5.f;
// 
// 		DrawDebugCapsule(GetWorld(),
// 			Center,
// 			HalfHeight,
// 			AttackRadius,
// 			CapsuleRot,
// 			DrawColor,
// 			false,
// 			DebugLitTime);
#endif

		if (bResult)
		{
// 			if (HitResult.GetActor()->IsValidLowLevel())
// 			{
// 				TPLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName());
// 
// 				FDamageEvent DmgEvent;
// 				HitResult.GetActor()->TakeDamage(GetFinalAttackDamage(), DmgEvent, GetController(), this);
// 			}


			FString FilterTag = "Player";
			if(bIsPlayer == false)
				FilterTag = "Enemy";
			for (const auto CurDetact : HitResult)
			{
				if (CurDetact.GetActor()->IsValidLowLevel() && CurDetact.GetActor()->Tags.Find(*FilterTag) == -1)
				{
					TPLOG(Warning, TEXT("Hit Actor Name : %s"), *CurDetact.GetActor()->GetName());

					float BulletDamage = GetFinalAttackDamage();
					float BulletCriticalRate = GetFinalCriticalRate();
					float BulletCriticalDamageRate = GetFinalCriticalDamageRate();
					float BulletPierce = GetFinalPierceRate();

					FDamageEvent DmgEvent;
					ATPCharacter* HitCharacter = Cast<ATPCharacter>(CurDetact.GetActor());
					if(HitCharacter == nullptr)
						continue;
					// 최종 공격력
					float FinalDamage = BulletDamage;

					// 최종 방어력
					float FinalDefence = HitCharacter->GetFinalDefencePoint();
					if (FinalDefence > 0)
					{
						FinalDefence -= BulletPierce;
						if (FinalDefence <= 0.f)
							FinalDefence = 0.f;
					}

					// 치명타
					float FinalCriticalRate = 1.f;
					if (FMath::FRandRange(0, 100.f) < BulletCriticalRate)
					{
						FinalCriticalRate = BulletCriticalDamageRate;
					}

					//캐릭터 데미지 = ((캐릭터 최종 공격력 * (몬스터 최종 방어력-캐릭터 방어 관통)) * (캐릭터 치명타 발생 여부 * 캐릭터 치명타 데미지 배율)					
					float CurFinalBulletDamage = FinalDamage * (1 - FinalDefence) * FinalCriticalRate;

					CurDetact.GetActor()->TakeDamage(CurFinalBulletDamage, DmgEvent, GetController(), this);
				}
			}
		}
		return;
	}
	else
	{
	// 총기들.
		if(CurrentWeapon->TryShotBullet() == false)
			return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();


	auto ABGameInstance = Cast<UTPGameInstance>(GetGameInstance());

	// 총알 생성 및 위치와 각도 세팅
	FVector BulletPos;
	FRotator BulletRot;
	FVector BulletTargetPos;


	CurrentWeapon->GetBulletSpawnPosNRot(BulletPos, BulletRot);
	if (bIsPlayer)
	{
		BulletTargetPos = GetBulletDirection(BulletPos);
		BulletRot = BulletTargetPos.Rotation();
	}
	else
	{
		BulletTargetPos = GetActorForwardVector();

		// 탄튐
		float RandomConeHalfAngle = FMath::DegreesToRadians(GetBulletSplatterAngle() / 2.f);
		BulletTargetPos = FMath::VRandCone(BulletTargetPos, RandomConeHalfAngle);
		BulletRot = BulletTargetPos.Rotation();
	}


	

// 	FVector TestTargetPos2 = BulletPos + BulletTargetPos * 100;
// 	DrawDebugLine(GetWorld(), BulletPos, TestTargetPos2, FColor::Blue, true, 1.f);

	auto NewBullet = GetWorld()->SpawnActor<ATPBullet>(ATPBullet::StaticClass(),BulletPos , BulletRot, SpawnParams);

	NewBullet->InitBullet(ABGameInstance->GetNextBulletIndex(), BulletSpd, this, bIsPlayer);

	// 반동 계산 및 적용.
	if (UseRecoil)
	{
		FVector2D NextRecoil = CurrentWeapon->GetCurRecoil();
		TargetRecoil.X = NextRecoil.X;
		TargetRecoil.Y = NextRecoil.Y;
		TargetRecoil *= CharacterStat->GetRecoil();
		CurRecoil = FVector::ZeroVector;
		NeedRecoil = true;
// 		AddControllerPitchInput(-CurRecoil.Y);
// 		AddControllerYawInput(-CurRecoil.X);
	}
}

void ATPCharacter::OnAssetLoadCompleted()
{
// 	USkeletalMesh* AssetsLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
// 	AssetStreamingHandle.Reset();
// 	TPCHECK(AssetsLoaded != nullptr);
// 	if (AssetsLoaded != nullptr)
// 	{
// 		GetMesh()->SetSkeletalMesh(AssetsLoaded);
// 	}

	CurRelativeLocLerpAds = FVector(0.f, 40.f, 80.f);
	PrevArmLength = 200.f;
	SetAds(true);
	SetCharacterState(ECharacterState::READY);
}
