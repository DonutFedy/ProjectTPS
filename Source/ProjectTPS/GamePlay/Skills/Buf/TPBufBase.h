// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Passive/TPPassiveBase.h"
#include "TPBufBase.generated.h"

USTRUCT(BlueprintType)
struct FTPEffectInfo
{
	GENERATED_BODY()
public:
	FTPEffectInfo() :StatType(EBufStatType::BST_NONE), StatValue(0.f) , _BufDuration(0.f), RealApplyValue(0.f)
	{

	}

	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBufStatType StatType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StatValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _BufDuration; // ���� ���ӽð�

	float RealApplyValue; // ���� ���� ��
};
/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPBufBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void InitBuf(FTPPassiveGroupTable& CurPassiveInfo, TObjectPtr<class UTPSkillComponent> InSkillComp);
	virtual bool AddEffect(FTPPassiveGroupTable& CurPassiveInfo);
	virtual void CheckBufTime(float DeltaTime);

	// return �� true�� ���� ���� ���Ŵ�.
	bool RemoveEffect(ESkillEffectType InRemoveTargetType);

protected:
	void ProcessBuf(FTPEffectInfo& NewEffect, TObjectPtr<class UTPCharacterStatComponent> InStatComp);
	void UnProcessBuf(FTPEffectInfo& NewEffect, TObjectPtr<class UTPCharacterStatComponent> InStatComp);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BufStat)
	TMap<EBufStatType, struct FTPEffectInfo> BufStatInfo;

	UPROPERTY()
	TObjectPtr<class UTPSkillComponent> SkillComp;
};
