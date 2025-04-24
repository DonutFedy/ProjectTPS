// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectTPS.h"
#include "TPSkillBufQueryBase.h"
#include "TPSkillBufQuery_AddStat.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPSkillBufQuery_AddStat : public UTPSkillBufQueryBase
{
	GENERATED_BODY()
public:
	virtual void ProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo) override;
	virtual void UnProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo) override;
	void SetBufQuery(TArray< struct FTPBufStatInfo>& InSetupInfo);
	//virtual FString GetTestLog();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BufStat)
	TArray< struct FTPBufStatInfo> BufStatInfo;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillType, Meta = (AllowprivateAccess=true))
	TArray<float> ArrRealAddStatValue;
};
