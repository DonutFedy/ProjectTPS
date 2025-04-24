// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectTPS.h"
#include "UObject/NoExportTypes.h"
#include "TPSkillBufQueryBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTTPS_API UTPSkillBufQueryBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void ProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo) {}
	virtual void UnProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo) {}
	//static UTPSkillBufQueryBase* BufQueryFactory(struct FTPSkillInfo InFromSkillInfo);
	virtual FString GetTestLog(){ return "None Buf"; }
protected:
	// 원래 스킬 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillType, Meta = (AllowprivateAccess=true))
	ESkillType SkillType;
//	int SkillIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillType, Meta = (AllowprivateAccess=true))
	EBufType CurBufType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillType, Meta = (AllowprivateAccess=true))
	EBufStatType CurBufStatType;
};
