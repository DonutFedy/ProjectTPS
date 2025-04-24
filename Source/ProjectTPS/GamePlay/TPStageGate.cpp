// Fill out your copyright notice in the Description page of Project Settings.


#include "TPStageGate.h"
#include "../TPGameInstance.h"
#include "../TPCharacter.h"

// Sets default values
ATPStageGate::ATPStageGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);


	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	MainMesh->SetupAttachment(CollisionComp);
	CollisionComp->InitBoxExtent(FVector(50,50,100));
	CollisionComp->SetCollisionProfileName(TEXT("Gate"));
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ATPStageGate::OnCharacterOverlap);
	CollisionComp->SetActive(false);
	CollisionComp->SetGenerateOverlapEvents(false);

	CollisionComp->SetRelativeLocation(FVector(0,0,75));
}

// Called when the game starts or when spawned
void ATPStageGate::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPStageGate::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsActiveGate && OtherActor && OtherActor != this)
	{
		ATPCharacter* HitCharacter = Cast<ATPCharacter>(OtherActor);
		if (HitCharacter->IsValidLowLevel()
			&& HitCharacter->bIsPlayer == true)
		{
			auto ABGameInstance = Cast<UTPGameInstance>(GetGameInstance());
			TPCHECK(ABGameInstance != nullptr);
			if (ABGameInstance != nullptr)
			{
				ABGameInstance->NextStage();
				IsActiveGate = false;
			}
		}
	}
}

void ATPStageGate::SetEnableCollision(bool InSetEnableCollision)
{
	SetActorHiddenInGame(!InSetEnableCollision);
	CollisionComp->SetGenerateOverlapEvents(InSetEnableCollision);
	CollisionComp->SetActive(InSetEnableCollision);
	IsActiveGate = InSetEnableCollision;
}

