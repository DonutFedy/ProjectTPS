// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "GameFramework/PlayerController.h"
#include "TPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API ATPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPPlayerController();

	virtual void PostInitializeComponents()			override;
	virtual void OnPossess(APawn* aPawn)		override;

	class UTPInGameUI* GetHUDWidget() const;
	class ATPPlayerState* GetCurrentPlayerState() const { return TPPlayerState; }

	void InitController();
	void InitTitle();
	
	UFUNCTION(Exec)
	void TP_AddSkill(int SkillIndex, int SkillLv);
	UFUNCTION(Exec)
	void TP_RemoveSkill(int SkillIndex);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UTPInGameUI> HUDWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UTPTitle> TitleUIWidgetClass;

private:
 	UPROPERTY()
 	class UTPInGameUI* HUDWidget;
	UPROPERTY()
	class UTPTitle* UITitleWidgetInstance;

	UPROPERTY()
	class ATPPlayerState* TPPlayerState;
};
