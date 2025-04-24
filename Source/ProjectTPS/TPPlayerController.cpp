// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPlayerController.h"
#include "TPInGameUI.h"
#include "TPPlayerState.h"
#include "TPCharacter.h"
#include "TPTitle.h"

ATPPlayerController::ATPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UTPInGameUI> UI_HUD_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
	static ConstructorHelpers::FClassFinder<UTPTitle> UI_TITLE_C(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Title.UI_Title_C'"));
	if (UI_TITLE_C.Succeeded())
	{
		TitleUIWidgetClass = UI_TITLE_C.Class;
	}
}

void ATPPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TPLOG_S(Warning);

}

void ATPPlayerController::OnPossess(APawn* aPawn)
{
	TPLOG_S(Warning);
	Super::OnPossess(aPawn);
}

class UTPInGameUI* ATPPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void ATPPlayerController::InitController()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	if(UITitleWidgetInstance)
		UITitleWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);

	if (HUDWidget == nullptr)
	{
		HUDWidget = CreateWidget<UTPInGameUI>(this, HUDWidgetClass);
		HUDWidget->AddToViewport();
	}

	TPPlayerState = Cast<ATPPlayerState>(PlayerState);
	TPCHECK(TPPlayerState != nullptr);
	HUDWidget->BindPlayerState(TPPlayerState);
	TPPlayerState->OnPlayerStateChanged.Broadcast();
}

void ATPPlayerController::InitTitle()
{
	if (UITitleWidgetInstance == nullptr)
	{
		UITitleWidgetInstance = CreateWidget<UTPTitle>(this, TitleUIWidgetClass);
		UITitleWidgetInstance->AddToViewport();
	}
	if (UITitleWidgetInstance)
		UITitleWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	if (HUDWidget)
		HUDWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ATPPlayerController::BeginPlay()
{
	Super::BeginPlay();




}
