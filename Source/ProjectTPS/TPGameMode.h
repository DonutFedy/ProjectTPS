// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "GameFramework/GameModeBase.h"
#include "TPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API ATPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPGameMode();
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;



};
