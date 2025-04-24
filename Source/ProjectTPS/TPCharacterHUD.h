// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPCharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPCharacterHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UTPCharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UTPCharacterStatComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
