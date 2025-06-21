// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectTPS.h"
#include "GameFramework/Actor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Sound/SoundCue.h"
#include "TPItem.generated.h"



UCLASS()
class PROJECTTPS_API ATPItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPItem();

	void SetItem(FVector spawnPos);
	void SetHidden();

	virtual void TakeToCharacter(TObjectPtr<class ATPCharacter> inChar);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// VFX
	void PlayEffect(); // 사운드랑 vfx 플레이
	// VFX end

public:
	void SetGetterTrigger(bool InAvailable);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	UStaticMeshComponent* itemMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	UStaticMesh* itemMesh;

	UPROPERTY(VisibleAnywhere, Category=Weapon)
	UBoxComponent* triggerColider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	bool isDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EFFECT)
	TArray<TObjectPtr<USoundCue>>	curSoundCue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EFFECT)
	TObjectPtr<class UNiagaraSystem> curMuzzleFlashFX;
};
