// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPTitle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPTitle : public UUserWidget
{
	GENERATED_BODY()
	


	
	UFUNCTION(BlueprintCallable, Category="Title")
	void StartGame();
};
