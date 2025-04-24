// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCharacterHUD.h"
#include "TPCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UTPCharacterHUD::BindCharacterStat(class UTPCharacterStatComponent* NewCharacterStat)
{
	TPCHECK(NewCharacterStat != nullptr);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddLambda([this]()->void {
		if (CurrentCharacterStat->IsValidLowLevel())
		{
			TPLOG(Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
		}
		});
	NewCharacterStat->OnHPChanged.AddUObject(this, &UTPCharacterHUD::UpdateHPWidget);
}

void UTPCharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	TPCHECK(HPProgressBar != nullptr);
	UpdateHPWidget();
}

void UTPCharacterHUD::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (HPProgressBar != nullptr)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
