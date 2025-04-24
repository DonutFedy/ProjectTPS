// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TPBullet.generated.h"

UCLASS()
class PROJECTTPS_API ATPBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnBulletDecal(const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents()						override;


	void InitBullet(int InIndex, int InBulletSpd, class ATPCharacter* InOwnerActor, bool InIsPlayerBullet);
	
    UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
    UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere, Category= Bullet)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category= Bullet)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = Bullet)
	UProjectileMovementComponent* Movement;

	UPROPERTY(VIsibleAnywhere, Category=Bullet)
	int BulletIndex;

	UPROPERTY(VIsibleAnywhere, Category = Bullet)
	int BulletType;

	UPROPERTY(VIsibleAnywhere, Category = Bullet)
	float BulletSpd;

	UPROPERTY(VIsibleAnywhere, Category = Bullet)
	float BulletDamage;
	UPROPERTY(VIsibleAnywhere, Category = Bullet)
	float BulletPierce;
	UPROPERTY(VIsibleAnywhere, Category = Bullet)
	float BulletCriticalRate;
	UPROPERTY(VIsibleAnywhere, Category = Bullet)
	float BulletCriticalDamageRate;

	UPROPERTY(Transient,VIsibleAnywhere, Category = Bullet)
	bool IsPlayersBullet;

	UPROPERTY(Transient, VIsibleAnywhere, Category = Bullet)
	class ATPCharacter* OwnerActor;

// 	UPROPERTY(VisibleAnywhere, Category = Movement)
// 	UFloatingPawnMovement* Movement;
};
