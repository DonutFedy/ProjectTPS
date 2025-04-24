// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectTPS.h"
#include "TPSkillBufQueryBase.h"
#include "TPSkillBufQuery_MulStat.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPSkillBufQuery_MulStat : public UTPSkillBufQueryBase
{
	GENERATED_BODY()

public:
	virtual void ProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo) override;
	virtual void UnProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo) override;
	void SetBufQuery(EBufStatType InStatType, float InMulStatValue);
	virtual FString GetTestLog();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillType, Meta = (AllowprivateAccess=true))
	float MulStatValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillType, Meta = (AllowprivateAccess=true))
	float RealAddStatValue;
};
