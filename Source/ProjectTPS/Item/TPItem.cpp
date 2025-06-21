// Fill out your copyright notice in the Description page of Project Settings.


#include "TPItem.h"
#include "../TPCharacter.h"
#include "../TPGameInstance.h"
#include "../Table/TPBulletRecoilData.h"
//#include "Table/TPItemTable.h"
#include "Engine/EngineTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "../Management/TPStageManager.h"


// Sets default values
ATPItem::ATPItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	triggerColider = CreateDefaultSubobject<UBoxComponent>(TEXT("WEAPON_GETTER"));
	itemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = itemMeshComp;

	triggerColider->SetupAttachment(RootComponent);
	triggerColider->SetBoxExtent(FVector(40.f, 42.f, 30.f));
	triggerColider->SetCollisionProfileName(TEXT("GetWeapon"));

// 	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ITEMBASE(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapon/Pistol/Mesh/SK_Pistol.SK_Pistol'"));
// 	if (SK_ITEMBASE.Succeeded())
// 	{
// 		itemMesh->SetSkeletalMesh(SK_ITEMBASE.Object);
// 	}
	itemMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	itemMeshComp->SetStaticMesh(itemMesh);
}

void ATPItem::SetItem(FVector spawnPos)
{
	SetGetterTrigger(true);
	SetActorEnableCollision(true);
	SetActorLocation(spawnPos);
	triggerColider->SetCollisionProfileName(TEXT("GetWeapon"));
	//itemMeshComp->SetStaticMesh(itemMesh);
	SetActorHiddenInGame(false);
	triggerColider->SetHiddenInGame(false, true);
	itemMeshComp->SetHiddenInGame(false, true);
	itemMeshComp->SetVisibility(true);
}

void ATPItem::SetHidden()
{
	SetActorHiddenInGame(true);
	triggerColider->SetCollisionProfileName(TEXT("NoCollision"));
	SetActorEnableCollision(false);
	SetGetterTrigger(false);
	triggerColider->SetHiddenInGame(true, true);
	itemMeshComp->SetHiddenInGame(true, true);
	itemMeshComp->SetVisibility(false);
}

void ATPItem::TakeToCharacter(TObjectPtr<ATPCharacter> inChar)
{
	TPCHECK(inChar != nullptr);
	inChar->TakeItem(this);
	SetHidden();
}

// Called when the game starts or when spawned
void ATPItem::BeginPlay()
{
	Super::BeginPlay();

	SetHidden();
}

void ATPItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	triggerColider->OnComponentBeginOverlap.AddDynamic(this, &ATPItem::OnCharacterOverlap);
}

void ATPItem::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TPLOG_S(Warning);

	auto TPCharacter = Cast<ATPCharacter>(OtherActor);
	TPCHECK(TPCharacter != nullptr);

	if (TPCharacter != nullptr && TPCharacter->IsPlayer())
	{
		TPCharacter->TakeItem(this);
		SetHidden();

		TakeToCharacter(TPCharacter);

		// Manager에게 리턴
		auto TPGameInstance = Cast<UTPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		TPCHECK(TPGameInstance != nullptr);
		TObjectPtr< UTPStageManager> StageMgr = TPGameInstance->GetStageManager();
		StageMgr->ReturnItemObj(this);
	}
}

void ATPItem::PlayEffect()
{

	if (curMuzzleFlashFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(curMuzzleFlashFX, itemMeshComp,
			TEXT("Muzzle"), FVector::ZeroVector, FRotator::ZeroRotator
			, EAttachLocation::SnapToTargetIncludingScale, true);
	}

	if (curSoundCue.Num())
	{
		UGameplayStatics::PlaySoundAtLocation(this, curSoundCue[FMath::RandRange(0, curSoundCue.Num() - 1)].Get(), GetActorLocation());
	}
}

void ATPItem::SetGetterTrigger(bool InAvailable)
{
	ECollisionEnabled::Type CollType = ECollisionEnabled::NoCollision;
	if (InAvailable)
		CollType = ECollisionEnabled::QueryOnly;
	triggerColider->SetCollisionEnabled(CollType);
}

void ATPItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
