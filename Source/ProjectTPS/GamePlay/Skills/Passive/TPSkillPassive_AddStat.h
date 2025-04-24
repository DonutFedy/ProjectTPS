// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TPSkillBase_Legacy.h"
#include "TPSkillPassive_AddStat.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTPS_API UTPSkillPassive_AddStat : public UTPSkillBase_Legacy
{
	GENERATED_BODY()
	
public:
	virtual void SetSkill(int InSkillIndex, int InSkillLevel, TObjectPtr< UTPSkillComponent> OwnComp) override;

	virtual void ProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo, TObjectPtr<UTPCharacterStatComponent> InStatComp) override;
	virtual void UnProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo, TObjectPtr<UTPCharacterStatComponent> InStatComp) override;
	virtual bool CheckBufTime(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BufStat, Meta = (AllowprivateAccess = true))
	TArray< FTPBufStatInfo> BufStatInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillType, Meta = (AllowprivateAccess=true))
	TArray<float> ArrRealAddStatValue;



	/*
	
실드							최대치 증가
체력							최대치 증가
스태미너						최대치 증가
공격력						무기 공격력 증가
방어력						캐릭터 방어력 증가
이동속도						이동속도 증가
치명타 확률					치명타 확률 증가
재장전 시간					재장전 시간 감소
투사체 속도 증가				투사체 속도 증가
정확도 증가					탄퍼짐 범위 감소
반동 감소					반동 수치 감소
구르기 무적시간 증가			구르기 시 무적시간(초) 증가
구르기 스태미너 소모 감소		"구르기에 소모되는 스태미너량 감소"
탄약 수량					최대 탄약 보유량 증가
발사 탄약 증가				탄창(매거진)의 최대량 증가
쿨타임						착용중인 모든 스킬 쿨타임 감소
스킬 스택					스택 가능한 스킬들의 수량 증가
자금							자금 획득 시 획득량 증가
	*/
};
