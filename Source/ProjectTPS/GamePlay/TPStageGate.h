// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectTPS.h"
#include "GameFramework/Actor.h"
#include "TPStageGate.generated.h"

UCLASS()
class PROJECTTPS_API ATPStageGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPStageGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SetEnableCollision(bool InSetEnableCollision);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Gate, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MainMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gate, Meta = (AllowPrivateAccess = true))
	UBoxComponent* CollisionComp;

	bool IsActiveGate;
};
