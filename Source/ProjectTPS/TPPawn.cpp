// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPawn.h"

// Sets default values
ATPPawn::ATPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(88.f);
	Capsule->SetCapsuleRadius(34.f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHAR(TEXT("/Script/Engine.SkeletalMesh'/Game/KoreanTraditionalMartialArts/Meshs/Characters/Meshs/SKM_Soldier_1.SKM_Soldier_1'"));
// 	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHAR(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Meshes/Aurora.Aurora'"));
// 	if (SK_CHAR.Succeeded())
// 	{
// 		Mesh->SetSkeletalMesh(SK_CHAR.Object);
// 	}


// 	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
// 	static ConstructorHelpers::FClassFinder<UAnimInstance> Study_ANIM(TEXT("/Script/Engine.AnimBlueprint'/Game/Book/Animations/StudyAnimBlueprint.StudyAnimBlueprint_C'"));
// 	if (Study_ANIM.Succeeded())
// 	{
// 		Mesh->SetAnimInstanceClass(Study_ANIM.Class);
// 	}
}

// Called when the game starts or when spawned
void ATPPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ATPPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ATPPawn::LeftRight);
}

void ATPPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TPLOG_S(Warning);
}

void ATPPawn::PossessedBy(AController* NewController)
{
	TPLOG_S(Warning);
	Super::PossessedBy(NewController);
}

void ATPPawn::UpDown(float NewAxisValue)
{
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void ATPPawn::LeftRight(float NewAxisValue)
{
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}

