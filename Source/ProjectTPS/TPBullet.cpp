// Fill out your copyright notice in the Description page of Project Settings.


#include "TPBullet.h"
#include "TPCharacter.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Pawn.h"

// Sets default values
ATPBullet::ATPBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!CollisionComp)
	{
		CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CAPSULE"));
		RootComponent = CollisionComp;
		CollisionComp->InitSphereRadius(15.0f);
		CollisionComp->SetCollisionProfileName(TEXT("Bullet"));
		CollisionComp->OnComponentHit.AddDynamic(this, &ATPBullet::OnHit);
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ATPBullet::OnCharacterOverlap);
		CollisionComp->SetActive(false);

		// 0번 머티리얼 슬롯에서 동적 머티리얼 인스턴스 생성
		UMaterialInstanceDynamic* DynamicMaterial = CollisionComp->CreateDynamicMaterialInstance(0);

		if (DynamicMaterial)
		{
			// 머티리얼의 VectorParameter (예: "BaseColor") 변경
			DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor::Red);
		}
	}
	if (!Mesh)
	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
		Mesh->SetupAttachment(CollisionComp);
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BULLET(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
		if (SM_BULLET.Succeeded())
		{
			Mesh->SetStaticMesh(SM_BULLET.Object);
		}
	}
	if (!Movement)
	{
		Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MOVEMENT"));
		Movement->SetUpdatedComponent(CollisionComp);
	}

	SetActorScale3D(FVector( 0.3f, 0.1f, 0.1f ));
	

}

// Called when the game starts or when spawned
void ATPBullet::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->MoveIgnoreActors.Add(GetOwner());
}

void ATPBullet::SpawnBulletDecal(const FHitResult& Hit)
{
	UMaterialInterface* DecalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/Weapon/Material/M_BulletDecal.M_BulletDecal'")); // 경로는 본인 머티리얼 경로로

	if (!DecalMaterial) return;

	FRotator DecalRotation = Hit.ImpactNormal.Rotation();
	DecalRotation.Pitch += 180.0f; // 반대 방향으로 투사

	// 디칼 스폰
	UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		DecalMaterial,
		FVector(20.0f, 5.0f, 5.0f), // 디칼 크기
		Hit.ImpactPoint,             // 충돌 지점
		DecalRotation,// 충돌면 방향
		10.0f                        // 생존 시간 (초)
	);
}

// Called every frame
void ATPBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPBullet::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void ATPBullet::InitBullet(int InIndex, int InBulletSpd, ATPCharacter* InOwnerActor, bool InIsPlayerBullet)
{
	SetActorHiddenInGame(false);

	// Data load by BulletType
	OwnerActor = InOwnerActor;
	BulletIndex = InIndex;
	//BulletType = BulletIndex;
	BulletSpd = InBulletSpd;
	BulletDamage = OwnerActor->GetFinalAttackDamage();
	BulletCriticalRate = OwnerActor->GetFinalCriticalRate();
	BulletCriticalDamageRate = OwnerActor->GetFinalCriticalDamageRate();
	BulletPierce = OwnerActor->GetFinalPierceRate();
	IsPlayersBullet = InIsPlayerBullet;


	// 총알 이동 설정
	Movement->SetUpdatedComponent(CollisionComp);
	Movement->InitialSpeed = BulletSpd;  // 초기 속도
	Movement->MaxSpeed = BulletSpd;
	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = false;  // 벽에 부딪히면 튕기지 않고 제거
	Movement->ProjectileGravityScale = 0.0f;

	Movement->Velocity = GetActorForwardVector() * BulletSpd;


	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(CollisionComp->GetMaterial(0));

	if (DynamicMaterial)
	{
		FLinearColor BulletColor = FLinearColor::Red;
		if(IsPlayersBullet)
			BulletColor = FLinearColor::Blue;
		// 머티리얼의 VectorParameter (예: "BaseColor") 변경
		DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), BulletColor);
	}

	CollisionComp->SetActive(true);
}



void ATPBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OwnerActor != OtherActor)
	{
		// 캐릭터와 충돌하면 데미지 적용
		ATPCharacter* HitCharacter = Cast<ATPCharacter>(OtherActor);
		if (HitCharacter->IsValidLowLevel() 
			&& ((IsPlayersBullet == true && HitCharacter->bIsPlayer == false)
			|| (IsPlayersBullet == false && HitCharacter->bIsPlayer == true)))
		{
			//UGameplayStatics::ApplyDamage(HitCharacter, BulletDamage, nullptr, this, nullptr); // 10 데미지
			FDamageEvent DmgEvent;

			

			// 최종 공격력
			float FinalDamage = BulletDamage;

			// 최종 방어력
			float FinalDefence = HitCharacter->GetFinalDefencePoint();
			if (FinalDefence > 0)
			{
				FinalDefence -= BulletPierce;
				if(FinalDefence<=0.f)
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

			FString CurDamageLog = FString::Printf(TEXT("%0.1f"), CurFinalBulletDamage);

			DrawDebugString(
				GetWorld(),
				Hit.ImpactPoint,          // 표시할 위치
				*CurDamageLog,           // 표시할 텍스트
				nullptr,                         // 소유 액터 (없으면 nullptr)
				FColor::Red,                    // 텍스트 색상
				0.5f,                            // 지속 시간
				true                             // 깊이 테스트 여부 (false면 벽 뒤에서도 보임)
				);
			HitCharacter->TakeDamage(CurFinalBulletDamage, DmgEvent, OwnerActor->GetController(), this);
		}


		SpawnBulletDecal(Hit);

		// 총알 제거
		Destroy();
	}
}

void ATPBullet::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
// 	if (OtherActor && OtherActor != this && OwnerActor != OtherActor)
// 	{
// 		// 캐릭터와 충돌하면 데미지 적용
// 		ATPCharacter* HitCharacter = Cast<ATPCharacter>(OtherActor);
// 		if (HitCharacter->IsValidLowLevel())
// 		{
// 			//UGameplayStatics::ApplyDamage(HitCharacter, BulletDamage, nullptr, this, nullptr); // 10 데미지
// 			FDamageEvent DmgEvent;
// 
// 
// 			// 최종 공격력
// 			float FinalDamage = BulletDamage;
// 
// 			// 최종 방어력
// 			float FinalDefence = HitCharacter->GetFinalDefencePoint();
// 
// 			// 치명타
// 			float FinalCriticalRate = 1.f;
// 			if (FMath::FRandRange(0, 100.f) < BulletCriticalRate)
// 			{
// 				FinalCriticalRate = BulletCriticalDamageRate;
// 			}			
// 
// 			//캐릭터 데미지 = ((캐릭터 최종 공격력 * (몬스터 최종 방어력-캐릭터 방어 관통)) * (캐릭터 치명타 발생 여부 * 캐릭터 치명타 데미지 배율)					
// 			float CurFinalBulletDamage = FinalDamage * (1- (FinalDefence - BulletPierce)) * FinalCriticalRate;
// 
// 			HitCharacter->TakeDamage(CurFinalBulletDamage, DmgEvent, OwnerActor->GetController(), this);
// 		}
// 
// 		// 총알 제거
// 		Destroy();
// 	}
}
