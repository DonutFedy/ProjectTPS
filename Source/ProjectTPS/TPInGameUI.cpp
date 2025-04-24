// Fill out your copyright notice in the Description page of Project Settings.


#include "TPInGameUI.h"


#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TPCharacterStatComponent.h"
#include "TPPlayerState.h"
#include "Components/Image.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"

void UTPInGameUI::BindCharacterState(class UTPCharacterStatComponent* CharacterStat)
{
	TPCHECK(CharacterStat != nullptr);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UTPInGameUI::UpdateCharacterStat);
	CharacterStat->OnShieldChanged.AddUObject(this, &UTPInGameUI::UpdateCharacterStat);
	CharacterStat->OnStaminaChanged.AddUObject(this, &UTPInGameUI::UpdateCharacterStat);
	CharacterStat->OnAccuracyChanged.AddUObject(this, &UTPInGameUI::UpdateAccuracyRate);

	NeedAccuracyUpdate = false;
}

void UTPInGameUI::BindPlayerState(class ATPPlayerState* PlayerState)
{
	TPCHECK(PlayerState != nullptr);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UTPInGameUI::UpdatePlayerState);
}


void UTPInGameUI::UpdateWaeponState(int32 ChargeAmmo, int32 RemainAmmo)
{
	CurrentChargeAmmoTxt->SetText(FText::FromString(FString::FromInt(ChargeAmmo)));
	RemainAmmoTxt->SetText(FText::FromString(FString::FromInt(RemainAmmo)));
}

float UTPInGameUI::GetAimImgSize()
{
	FVector2D Size = CrossHairBar->GetCachedGeometry().GetLocalSize();
	return Size.X / 2.f;
	//return 100.f;
}

void UTPInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	TPCHECK(HPBar != nullptr);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	TPCHECK(ExpBar != nullptr);

	StaminaBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbStamina")));
	TPCHECK(ExpBar != nullptr);

	ShieldBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbShield")));
	TPCHECK(ExpBar != nullptr);

// 	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
// 	TPCHECK(PlayerName != nullptr);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	TPCHECK(PlayerLevel != nullptr);

	CurrentChargeAmmoTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	TPCHECK(CurrentChargeAmmoTxt != nullptr);

	RemainAmmoTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	TPCHECK(RemainAmmoTxt != nullptr);

	CrossHairBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbCrossHair")));
	TPCHECK(CrossHairBar != nullptr);

	CrossHair_Cross.Empty();
	UImage* FindCrossHair_Cross = Cast<UImage>(GetWidgetFromName(TEXT("CrossHair_D")));
	TPCHECK(FindCrossHair_Cross != nullptr);
	CrossHair_Cross.Add(Cast<UCanvasPanelSlot>(FindCrossHair_Cross->Slot));
	FindCrossHair_Cross = Cast<UImage>(GetWidgetFromName(TEXT("CrossHair_U")));
	TPCHECK(FindCrossHair_Cross != nullptr);
	CrossHair_Cross.Add(Cast<UCanvasPanelSlot>(FindCrossHair_Cross->Slot));
	FindCrossHair_Cross = Cast<UImage>(GetWidgetFromName(TEXT("CrossHair_L")));
	TPCHECK(FindCrossHair_Cross != nullptr);
	CrossHair_Cross.Add(Cast<UCanvasPanelSlot>(FindCrossHair_Cross->Slot));
	FindCrossHair_Cross = Cast<UImage>(GetWidgetFromName(TEXT("CrossHair_R")));
	TPCHECK(FindCrossHair_Cross != nullptr);
	CrossHair_Cross.Add(Cast<UCanvasPanelSlot>(FindCrossHair_Cross->Slot));

	CurrentChargeAmmoTxt->SetText(FText::FromString("0"));
	RemainAmmoTxt->SetText(FText::FromString("0"));
}

void UTPInGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (NeedAccuracyUpdate)
	{
		float Ratio = CurrentCharacterStat->GetAccuracyRatio();
		float OffsetLot = Ratio * 50.f;
		// Down, Up, Left, Right
		CrossHair_Cross[0]->SetPosition(FVector2D(0.f, OffsetLot));
		CrossHair_Cross[1]->SetPosition(FVector2D(0.f, -OffsetLot));
		CrossHair_Cross[3]->SetPosition(FVector2D(-OffsetLot,0.f));
		CrossHair_Cross[2]->SetPosition(FVector2D(OffsetLot,0.f));
		CrossHairBar->SetPercent(Ratio);
	}
}

void UTPInGameUI::UpdateCharacterStat()
{
	TPCHECK(CurrentCharacterStat.IsValid());

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	StaminaBar->SetPercent(CurrentCharacterStat->GetStaminaRatio());
	ShieldBar->SetPercent(CurrentCharacterStat->GetShieldRatio());
}

void UTPInGameUI::UpdatePlayerState()
{
	TPCHECK(CurrentPlayerState != nullptr);

	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	//PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
}

void UTPInGameUI::UpdateAccuracyRate()
{
	NeedAccuracyUpdate = true;
}
