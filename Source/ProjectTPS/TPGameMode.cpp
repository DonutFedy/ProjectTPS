// Fill out your copyright notice in the Description page of Project Settings.


#include "TPGameMode.h"
#include "TPPawn.h"
#include "TPCharacter.h"
#include "TPPlayerController.h"
#include "TPPlayerState.h"
//#include "TPGameState.h"

ATPGameMode::ATPGameMode()
{
 	//DefaultPawnClass = ATPCharacter::StaticClass();
 	DefaultPawnClass = nullptr;
 	PlayerControllerClass = ATPPlayerController::StaticClass();
 	PlayerStateClass = ATPPlayerState::StaticClass();
}

void ATPGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
//	TPGameState = Cast<ATPGameState>(GameState);
}

void ATPGameMode::PostLogin(APlayerController* NewPlayer)
{
	TPLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	TPLOG(Warning, TEXT("PostLogin End"));
	ATPPlayerController* PlayerController = Cast<ATPPlayerController>(NewPlayer);
	if (PlayerController)
		PlayerController->InitTitle();
}
