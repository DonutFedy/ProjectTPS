// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTPS.h"
#include "Management/TPStageManager.h"
#include "TPCharacterStatComponent.h"
#include "Components/ActorComponent.h"
#include "TPSkillComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTTPS_API UTPSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSkillComponent();

	void SetSkillComponent(TObjectPtr <UTPCharacterStatComponent> InCharStateComp , TArray<FTPSkillInitData>& InStartSkills);
	void AddSkill(FTPSkillInitData& InAddSkillInfo);

	bool IsHaveSkill(int SkillIndex, ESkillType InSkillType);
	int GetSkillLevel(int SkillIndex, ESkillType InSkillType);

	float GetSkillColdTime(int SkillIndex);

	void TickSkillComponent(float DeltaTime);

	void AddBufQuery(TObjectPtr<class UTPSkillBase_Legacy> NewBuf);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<TObjectPtr<class UTPSkillBase_Legacy>> ArrActiveSkills;
	UPROPERTY()
	TArray<TObjectPtr<class UTPSkillBase_Legacy>> ArrPassiveSkills;
	UPROPERTY()
	TArray<TObjectPtr<class UTPSkillBase_Legacy>> ArrBufQueries;
	UPROPERTY()
	TObjectPtr<UTPCharacterStatComponent> spStatComp;

};
