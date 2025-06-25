// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../../ProjectTPS.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TPHealZone.generated.h"

UCLASS()
class PROJECTTPS_API ATPHealZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPHealZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnDecal(const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents()						override;

	// VFX
	void PlayEffect(); // 사운드랑 vfx 플레이
	UFUNCTION()
	void OnVFXFinished(class UNiagaraComponent* PSystem);
	// VFX end

	// 효과
	void EffectHeal();

	void InitHealZone(TObjectPtr<class UTPActiveBase> InOwnSkill, float InHealZoneValue, float inHealZonePeriod, float InRange , class ATPCharacter* InOwnerActor, bool InIsPlayerHealZone);
	
    UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
    UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
    UFUNCTION()
	void OnCharacterOverlapOut(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category= HealZone)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category= HealZone)
	UStaticMeshComponent* CurMesh;

	UPROPERTY(VIsibleAnywhere, Category = HealZone)
	float HealZoneValue;
	UPROPERTY(VIsibleAnywhere, Category = HealZone)
	float HealZonePeriod;

	float preiodTime;
	float curTime;

	UPROPERTY(Transient,VIsibleAnywhere, Category = HealZone)
	bool IsPlayersHealZone; // false면 모든 오브젝트 회복

	UPROPERTY(Transient, VIsibleAnywhere, Category = HealZone)
	class ATPCharacter* OwnerActor;
	UPROPERTY(Transient, VIsibleAnywhere, Category = HealZone)
	TArray<TObjectPtr<class ATPCharacter>> OverlapChars;
	
	UPROPERTY(Transient, VIsibleAnywhere, Category = HealZone)
	TObjectPtr<class UTPActiveBase> OwnSkill;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EFFECT, Meta = (AllowprivateAccess = true))
	TArray<TObjectPtr<USoundCue>>	CurSoundCue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EFFECT, Meta = (AllowprivateAccess = true))
	TObjectPtr<class UNiagaraSystem> MuzzleFlashFX;

};
