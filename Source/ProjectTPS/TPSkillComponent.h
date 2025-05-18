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
	void RemoveSkill(int RemoveSkillIndex);

	void TrigSkillCondition(ESkillConditionType InCheckSkillTriggerType);// 특정 컨디션에 해당되는 스킬들을 트리거 시킨다.
	bool IsHaveSkill(ESkillIndex SkillIndex);
	int GetSkillLevel(ESkillIndex SkillIndex);
	int GetSkillEffectValue(ESkillIndex SkillIndex, ESkillEffectType InEffectType);

	float GetSkillColdTime(int SkillIndex);

	void TickSkillComponent(float DeltaTime);

	void _TestLog(float DeltaTime);

	// return : 할당된 버프 index  
	// SkillSerializeIndex : 각 스킬들 할당된 고유값.
	bool AddBuf(int SkillSerializeIndex , FTPPassiveGroupTable& NewBufInfo);
	void RemoveEffect(int SkillSerializeIndex, ESkillEffectType InRemoveTargetType);
	
	TObjectPtr<UTPCharacterStatComponent> GetCharStatComp(){return spStatComp;}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TObjectPtr<class ATPCharacter> GetOwnChar();
protected:
private:
	UPROPERTY()
	TMap<int,TObjectPtr<class UTPBufBase>> MapBuf;


	UPROPERTY()
	TArray<TObjectPtr<class UTPSkillController>> ArrSkillController;

	UPROPERTY()
	TObjectPtr<UTPCharacterStatComponent> spStatComp;

};
