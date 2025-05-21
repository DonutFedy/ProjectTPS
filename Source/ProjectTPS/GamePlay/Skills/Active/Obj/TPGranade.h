// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../../ProjectTPS.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TPGranade.generated.h"

UCLASS()
class PROJECTTPS_API ATPGranade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPGranade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnGranadeDecal(const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents()						override;

	// VFX
	void PlayEffect(); // 사운드랑 vfx 플레이
	UFUNCTION()
	void OnVFXFinished(class UNiagaraComponent* PSystem);
	// VFX end

	void InitGranade(TObjectPtr<class UTPActiveBase> InOwnSkill, float InGranadeDamage, float InGranadeSpd, float InRange , class ATPCharacter* InOwnerActor, bool InIsPlayerGranade);
	
    UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
    UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere, Category= Granade)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category= Granade)
	UStaticMeshComponent* CurMesh;

	UPROPERTY(VisibleAnywhere, Category = Granade)
	UProjectileMovementComponent* Movement;

	UPROPERTY(VIsibleAnywhere, Category=Granade)
	int GranadeIndex;

	UPROPERTY(VIsibleAnywhere, Category = Granade)
	int GranadeType;

	UPROPERTY(VIsibleAnywhere, Category = Granade)
	float GranadeSpd;
	UPROPERTY(VIsibleAnywhere, Category = Granade)
	float GranadeRange;

	UPROPERTY(VIsibleAnywhere, Category = Granade)
	float GranadeDamage;
	UPROPERTY(VIsibleAnywhere, Category = Granade)
	float GranadePierce;
	UPROPERTY(VIsibleAnywhere, Category = Granade)
	float GranadeCriticalRate;
	UPROPERTY(VIsibleAnywhere, Category = Granade)
	float GranadeCriticalDamageRate;

	UPROPERTY(Transient,VIsibleAnywhere, Category = Granade)
	bool IsPlayersGranade;

	UPROPERTY(Transient, VIsibleAnywhere, Category = Granade)
	class ATPCharacter* OwnerActor;
	UPROPERTY(Transient, VIsibleAnywhere, Category = Granade)
	class AActor* PrevTarget;
	UPROPERTY(Transient, VIsibleAnywhere, Category = Granade)
	int BounceGranadeEffectValue;
	
	UPROPERTY(Transient, VIsibleAnywhere, Category = Granade)
	TObjectPtr<class UTPActiveBase> OwnSkill;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EFFECT)
	TArray<TObjectPtr<USoundCue>>	CurSoundCue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EFFECT)
	TObjectPtr<class UNiagaraSystem> MuzzleFlashFX;

// 	UPROPERTY(VisibleAnywhere, Category = Movement)
// 	UFloatingPawnMovement* Movement;
};
