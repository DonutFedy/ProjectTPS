// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TPCharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=ProjectTPS)
class PROJECTTPSSETTING_API UTPCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UTPCharacterSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;

};
