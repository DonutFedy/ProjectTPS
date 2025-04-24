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

		// 0�� ��Ƽ���� ���Կ��� ���� ��Ƽ���� �ν��Ͻ� ����
		UMaterialInstanceDynamic* DynamicMaterial = CollisionComp->CreateDynamicMaterialInstance(0);

		if (DynamicMaterial)
		{
			// ��Ƽ������ VectorParameter (��: "BaseColor") ����
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
	UMaterialInterface* DecalMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/Weapon/Material/M_BulletDecal.M_BulletDecal'")); // ��δ� ���� ��Ƽ���� ��η�

	if (!DecalMaterial) return;

	FRotator DecalRotation = Hit.ImpactNormal.Rotation();
	DecalRotation.Pitch += 180.0f; // �ݴ� �������� ����

	// ��Į ����
	UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		DecalMaterial,
		FVector(20.0f, 5.0f, 5.0f), // ��Į ũ��
		Hit.ImpactPoint,             // �浹 ����
		DecalRotation,// �浹�� ����
		10.0f                        // ���� �ð� (��)
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


	// �Ѿ� �̵� ����
	Movement->SetUpdatedComponent(CollisionComp);
	Movement->InitialSpeed = BulletSpd;  // �ʱ� �ӵ�
	Movement->MaxSpeed = BulletSpd;
	Movement->bRotationFollowsVelocity = true;
	Movement->bShouldBounce = false;  // ���� �ε����� ƨ���� �ʰ� ����
	Movement->ProjectileGravityScale = 0.0f;

	Movement->Velocity = GetActorForwardVector() * BulletSpd;


	UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(CollisionComp->GetMaterial(0));

	if (DynamicMaterial)
	{
		FLinearColor BulletColor = FLinearColor::Red;
		if(IsPlayersBullet)
			BulletColor = FLinearColor::Blue;
		// ��Ƽ������ VectorParameter (��: "BaseColor") ����
		DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), BulletColor);
	}

	CollisionComp->SetActive(true);
}



void ATPBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OwnerActor != OtherActor)
	{
		// ĳ���Ϳ� �浹�ϸ� ������ ����
		ATPCharacter* HitCharacter = Cast<ATPCharacter>(OtherActor);
		if (HitCharacter->IsValidLowLevel() 
			&& ((IsPlayersBullet == true && HitCharacter->bIsPlayer == false)
			|| (IsPlayersBullet == false && HitCharacter->bIsPlayer == true)))
		{
			//UGameplayStatics::ApplyDamage(HitCharacter, BulletDamage, nullptr, this, nullptr); // 10 ������
			FDamageEvent DmgEvent;

			

			// ���� ���ݷ�
			float FinalDamage = BulletDamage;

			// ���� ����
			float FinalDefence = HitCharacter->GetFinalDefencePoint();
			if (FinalDefence > 0)
			{
				FinalDefence -= BulletPierce;
				if(FinalDefence<=0.f)
					FinalDefence = 0.f;
			}

			// ġ��Ÿ
			float FinalCriticalRate = 1.f;
			if (FMath::FRandRange(0, 100.f) < BulletCriticalRate)
			{
				FinalCriticalRate = BulletCriticalDamageRate;
			}

			//ĳ���� ������ = ((ĳ���� ���� ���ݷ� * (���� ���� ����-ĳ���� ��� ����)) * (ĳ���� ġ��Ÿ �߻� ���� * ĳ���� ġ��Ÿ ������ ����)					
			float CurFinalBulletDamage = FinalDamage * (1 - FinalDefence) * FinalCriticalRate;

			FString CurDamageLog = FString::Printf(TEXT("%0.1f"), CurFinalBulletDamage);

			DrawDebugString(
				GetWorld(),
				Hit.ImpactPoint,          // ǥ���� ��ġ
				*CurDamageLog,           // ǥ���� �ؽ�Ʈ
				nullptr,                         // ���� ���� (������ nullptr)
				FColor::Red,                    // �ؽ�Ʈ ����
				0.5f,                            // ���� �ð�
				true                             // ���� �׽�Ʈ ���� (false�� �� �ڿ����� ����)
				);
			HitCharacter->TakeDamage(CurFinalBulletDamage, DmgEvent, OwnerActor->GetController(), this);
		}


		SpawnBulletDecal(Hit);

		// �Ѿ� ����
		Destroy();
	}
}

void ATPBullet::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
// 	if (OtherActor && OtherActor != this && OwnerActor != OtherActor)
// 	{
// 		// ĳ���Ϳ� �浹�ϸ� ������ ����
// 		ATPCharacter* HitCharacter = Cast<ATPCharacter>(OtherActor);
// 		if (HitCharacter->IsValidLowLevel())
// 		{
// 			//UGameplayStatics::ApplyDamage(HitCharacter, BulletDamage, nullptr, this, nullptr); // 10 ������
// 			FDamageEvent DmgEvent;
// 
// 
// 			// ���� ���ݷ�
// 			float FinalDamage = BulletDamage;
// 
// 			// ���� ����
// 			float FinalDefence = HitCharacter->GetFinalDefencePoint();
// 
// 			// ġ��Ÿ
// 			float FinalCriticalRate = 1.f;
// 			if (FMath::FRandRange(0, 100.f) < BulletCriticalRate)
// 			{
// 				FinalCriticalRate = BulletCriticalDamageRate;
// 			}			
// 
// 			//ĳ���� ������ = ((ĳ���� ���� ���ݷ� * (���� ���� ����-ĳ���� ��� ����)) * (ĳ���� ġ��Ÿ �߻� ���� * ĳ���� ġ��Ÿ ������ ����)					
// 			float CurFinalBulletDamage = FinalDamage * (1- (FinalDefence - BulletPierce)) * FinalCriticalRate;
// 
// 			HitCharacter->TakeDamage(CurFinalBulletDamage, DmgEvent, OwnerActor->GetController(), this);
// 		}
// 
// 		// �Ѿ� ����
// 		Destroy();
// 	}
}
