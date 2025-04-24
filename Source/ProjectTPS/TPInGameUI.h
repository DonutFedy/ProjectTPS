// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "Blueprint/UserWidget.h"
#include "TPInGameUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPInGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterState(class UTPCharacterStatComponent* CharacterStat);
	void BindPlayerState(class ATPPlayerState* PlayerState);

	void UpdateWaeponState(int32 ChargeAmmo, int32 RemainAmmo);
	float GetAimImgSize();
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void UpdateCharacterStat();
	void UpdatePlayerState();
	void UpdateAccuracyRate();

private:
	TWeakObjectPtr<class UTPCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class ATPPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* StaminaBar;

	UPROPERTY()
	class UProgressBar* ShieldBar;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* PlayerName;

	UPROPERTY()
	class UTextBlock* PlayerLevel;

	UPROPERTY()
	class UTextBlock* CurrentChargeAmmoTxt;

	UPROPERTY()
	class UTextBlock* RemainAmmoTxt;
	UPROPERTY()
	TArray<TObjectPtr< class UCanvasPanelSlot>> CrossHair_Cross;

	UPROPERTY()
	class UProgressBar* CrossHairBar;

	bool NeedAccuracyUpdate;
};
