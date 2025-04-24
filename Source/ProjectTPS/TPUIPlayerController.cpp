// Fill out your copyright notice in the Description page of Project Settings.


#include "TPUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void ATPUIPlayerController::BeginPlay()
{
	Super::BeginPlay();


 	TPCHECK(TitleUIWidgetClass);
 
	UITitleWidgetInstance = CreateWidget<UUserWidget>(this, TitleUIWidgetClass);
	TPCHECK(UITitleWidgetInstance != nullptr);

	UITitleWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UITitleWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}
