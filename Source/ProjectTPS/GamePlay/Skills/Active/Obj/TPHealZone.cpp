// Fill out your copyright notice in the Description page of Project Settings.


#include "TPHealZone.h"
#include "../../../../TPCharacter.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Pawn.h"
#include "../../../../TPGameInstance.h"
#include "../../../../Management/TPStageManager.h"
#include "../TPActiveBase.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"

// Sets default values
ATPHealZone::ATPHealZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CollisionComp)
	{
		CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CAPSULE"));
		RootComponent = CollisionComp;
		CollisionComp->InitSphereRadius(15.0f);
		CollisionComp->SetCollisionProfileName(TEXT("HealZone"));
		CollisionComp->SetCanEverAffectNavigation(false);
		//CollisionComp->OnComponentHit.AddDynamic(this, &ATPHealZone::OnHit);
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ATPHealZone::OnCharacterOverlap);
		CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ATPHealZone::OnCharacterOverlapOut);
		CollisionComp->SetActive(false);

		// 0번 머티리얼 슬롯에서 동적 머티리얼 인스턴스 생성
		UMaterialInstanceDynamic* DynamicMaterial = CollisionComp->CreateDynamicMaterialInstance(0);

		if (DynamicMaterial)
		{
			// 머티리얼의 VectorParameter (예: "BaseColor") 변경
			DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor::Red);
		}
	}
	if (!CurMesh)
	{
		CurMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
		CurMesh->SetupAttachment(CollisionComp);
		CurMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CurMesh->SetCanEverAffectNavigation(false);
		//Mesh->SetCollisionObjectType(ECollisionChannel::)
		//Mesh->SetCollisionEnabled( ECollisionEnabled::NoCollision);
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_HealZone(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
		if (SM_HealZone.Succeeded())
		{
			CurMesh->SetStaticMesh(SM_HealZone.Object);
		}
	}

	CollisionComp->SetActive(false);
	SetActorEnableCollision(false);

	SetActorScale3D(FVector( 1.f, 1.f, 1.f ));
}

// Called when the game starts or when spawned
void ATPHealZone::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->MoveIgnoreActors.Add(GetOwner());
}

void ATPHealZone::SpawnDecal(const FHitResult& Hit)
{
	UMaterialInterface* DecalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/Weapon/Material/M_HealZoneDecal.M_HealZoneDecal'")); // 경로는 본인 머티리얼 경로로

	if (!DecalMaterial) return;

	FRotator DecalRotation = Hit.ImpactNormal.Rotation();
	DecalRotation.Pitch += 180.0f; // 반대 방향으로 투사

	// 디칼 스폰
	UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		DecalMaterial,
		FVector(20.0f, 50.0f, 50.0f), // 디칼 크기
		Hit.ImpactPoint,             // 충돌 지점
		DecalRotation,// 충돌면 방향
		10.0f                        // 생존 시간 (초)
	);
}

// Called every frame
void ATPHealZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	preiodTime +=DeltaTime;
	curTime +=DeltaTime;

	if (curTime >= 1.f)
	{
		curTime -= 1.f;
		// 회복
		EffectHeal();
	}
	if (preiodTime >= HealZonePeriod)
	{
		// 회수.
		SetActorHiddenInGame(true);
		SetActorTickEnabled(false);
		SetActorEnableCollision(false);
		OwnSkill->ReleaseObj(this);
		return;
	}
}

void ATPHealZone::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void ATPHealZone::PlayEffect()
{
	if (MuzzleFlashFX)
	{
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			MuzzleFlashFX,
			CurMesh->GetComponentLocation(),     // 정확한 위치 지정
			CurMesh->GetComponentRotation(),     // 방향
			FVector(1.0f),                              // 스케일
			true,                                       // AutoDestroy
			true                                        // AutoActivate
			);

		if (NiagaraComp)
		{
			NiagaraComp->OnSystemFinished.AddDynamic(this, &ATPHealZone::OnVFXFinished);
		}
// 		else
// 		{
// 			// 총알 제거
// 			OwnSkill->ReleaseObj(this);
// 		}
	}
// 	else
// 	{
// 		// 총알 제거
// 		OwnSkill->ReleaseObj(this);
// 	}

	if (CurSoundCue.Num())
	{
		UGameplayStatics::PlaySoundAtLocation(this, CurSoundCue[FMath::RandRange(0, CurSoundCue.Num() - 1)].Get(), GetActorLocation());
	}
}

void ATPHealZone::OnVFXFinished(UNiagaraComponent* PSystem)
{
}

void ATPHealZone::EffectHeal()
{
	PlayEffect();

	for (auto overlapChar : OverlapChars)
	{
		overlapChar->GetCharStat()->RecoverHP(HealZoneValue);
	}
}

void ATPHealZone::InitHealZone(TObjectPtr<class UTPActiveBase> InOwnSkill, float InHealZoneValue, float inHealZonePeriod, float InRange, class ATPCharacter* InOwnerActor, bool InIsPlayerHealZone)
{
	SetActorHiddenInGame(false);
	PrimaryActorTick.bCanEverTick = true;
	OwnSkill = InOwnSkill;

	// Data load by HealZoneType
	OwnerActor = InOwnerActor;
	IsPlayersHealZone = InIsPlayerHealZone;
	HealZoneValue = InHealZoneValue;
	HealZonePeriod = inHealZonePeriod;

	TPCHECK(OwnerActor);
	OverlapChars.Empty();

	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(CollisionComp->GetMaterial(0));

	if (DynamicMaterial)
	{
		FLinearColor HealZoneColor = FLinearColor::Red;
		if (IsPlayersHealZone)
			HealZoneColor = FLinearColor::Blue;
		// 머티리얼의 VectorParameter (예: "BaseColor") 변경
		DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), HealZoneColor);
	}

	CollisionComp->SetActive(true);
	SetActorEnableCollision(true);

	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
}

void ATPHealZone::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	return;
}

void ATPHealZone::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor != this)
	{
		ATPCharacter* overlapCharacter = Cast<ATPCharacter>(OtherActor);

		TPCHECK(overlapCharacter != nullptr);
		if ((IsPlayersHealZone && overlapCharacter->IsPlayer())
		|| (!IsPlayersHealZone && !overlapCharacter->IsPlayer()))
		{
			OverlapChars.Add(overlapCharacter);
		}


// 		auto TPGameInstance = Cast<UTPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
// 		TPCHECK(TPGameInstance != nullptr);
// 		TObjectPtr< UTPStageManager> StageMgr = TPGameInstance->GetStageManager();
// 		TPCHECK(StageMgr != nullptr);
// 
// 		TArray<TObjectPtr<ATPCharacter>> ArrTarget;
// 		// 범위 내의 모든 캐릭터에게 데미지
// 		if (IsPlayersHealZone)
// 		{
// 			ArrTarget = StageMgr->GetEnemies();
// 		}
// 		else
// 		{
// 			ArrTarget = StageMgr->GetMainCharacter();
// 		}
// 
// 		if (ArrTarget.Num() > 0)
// 		{
// 			for (TObjectPtr<ATPCharacter> CurChar : ArrTarget)
// 			{
// 				// 캐릭터와 충돌하면 데미지 적용
// 				ATPCharacter* HitCharacter = CurChar;
// 
// 
// 				if (HitCharacter->IsValidLowLevel())
// 				{
// 					float Dist = FVector::Distance(GetActorLocation(), HitCharacter->GetActorLocation());
// 					if(Dist > HealZoneRange)
// 						continue;
// 
// 					//UGameplayStatics::ApplyDamage(HitCharacter, HealZoneDamage, nullptr, this, nullptr); // 10 데미지
// 					FDamageEvent DmgEvent;
// 					// 최종 공격력
// 					float FinalDamage = HealZoneDamage;
// 
// 					// 최종 방어력
// 					float FinalDefence = HitCharacter->GetFinalDefencePoint();
// 					if (FinalDefence > 0)
// 					{
// 						FinalDefence -= HealZonePierce;
// 						if (FinalDefence <= 0.f)
// 							FinalDefence = 0.f;
// 					}
// 
// 					// 치명타
// 					float FinalCriticalRate = 1.f;
// 					if (FMath::FRandRange(0, 100.f) < HealZoneCriticalRate)
// 					{
// 						FinalCriticalRate = HealZoneCriticalDamageRate;
// 					}
// 
// 					//캐릭터 데미지 = ((캐릭터 최종 공격력 * (몬스터 최종 방어력-캐릭터 방어 관통)) * (캐릭터 치명타 발생 여부 * 캐릭터 치명타 데미지 배율)					
// 					float CurFinalHealZoneDamage = FinalDamage * (1 - FinalDefence) * FinalCriticalRate;
// 
// 					FString CurDamageLog = FString::Printf(TEXT("%0.1f"), CurFinalHealZoneDamage);
// 
// 
// 					DrawDebugString(
// 						GetWorld(),
// 						SweepResult.ImpactPoint,          // 표시할 위치
// 						*CurDamageLog,           // 표시할 텍스트
// 						nullptr,                         // 소유 액터 (없으면 nullptr)
// 						FColor::Red,                    // 텍스트 색상
// 						0.5f,                            // 지속 시간
// 						true                             // 깊이 테스트 여부 (false면 벽 뒤에서도 보임)
// 					);
// 					HitCharacter->TakeDamage(CurFinalHealZoneDamage, DmgEvent, OwnerActor != nullptr ? OwnerActor->GetController() : nullptr, this);
// 					HitCharacter->PlayHitVFX(SweepResult);
// 				}
// 			}
// 		}
// 
// 		SpawnHealZoneDecal(SweepResult);
// 		bool bNeedDestory = true;
// 		// 총알을 튕겨야하는가?
// // 		if (BounceHealZoneEffectValue > 0)
// // 		{
// // 			--BounceHealZoneEffectValue;
// // 			// 500 범위의 적을 찾는다.
// // 			auto TPGameInstance = Cast<UTPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
// // 			TPCHECK(TPGameInstance != nullptr);
// // 			TObjectPtr< UTPStageManager> StageMgr = TPGameInstance->GetStageManager();
// // 			TPCHECK(StageMgr != nullptr);
// // 			if (StageMgr)
// // 			{
// // 				TObjectPtr<ATPCharacter> FindNearEnemy = StageMgr->GetNearEnemy(HitCharacter, SweepResult.ImpactPoint);
// // 				if (FindNearEnemy != nullptr)
// // 				{
// // 					float Dist = FVector::Distance(FindNearEnemy->GetActorLocation(), SweepResult.ImpactPoint);
// // 					if (Dist <= 500.f)
// // 					{
// // 						// 현재 부딪힌 엑터를 더이상 부딪히지 않게 세팅.
// // 						PrevTarget = HitCharacter;
// // 
// // 						bNeedDestory = false;
// // 						DrawDebugLine(GetWorld(), SweepResult.ImpactPoint, FindNearEnemy->GetActorLocation(), FColor::Yellow, false, 1.5f);
// // 						FVector NewDir = (FindNearEnemy->GetActorLocation() - SweepResult.ImpactPoint);
// // 						SetActorRotation(NewDir.Rotation());
// // 						Movement->Velocity = NewDir * HealZoneSpd;
// // 					}
// // 				}
// // 			}
// // 		}
// 		if (bNeedDestory)
// 		{
// 			// 총알 제거
// 			//Destroy();
// 			SetActorHiddenInGame(true);
// 			SetActorTickEnabled(false);
// 			Movement->Deactivate();
// 			PlayEffect();
// 		}
	}
}

void ATPHealZone::OnCharacterOverlapOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		ATPCharacter* overlapOutCharacter = Cast<ATPCharacter>(OtherActor);

		TPCHECK(overlapOutCharacter != nullptr);
		OverlapChars.Remove(overlapOutCharacter);
	}
}
