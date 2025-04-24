// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "GameFramework/PlayerController.h"
#include "TPUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API ATPUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> TitleUIWidgetClass;

	UPROPERTY()
	class UUserWidget* UITitleWidgetInstance;
};
