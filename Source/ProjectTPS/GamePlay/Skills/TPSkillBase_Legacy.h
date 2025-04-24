// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../ProjectTPS.h"
#include "Components/ActorComponent.h"
#include "../TPSkillBufQueryBase.h"
#include "../../TPSkillComponent.h"
//#include "../../TPCharacterStatComponent.h"
#include "TPSkillBase_Legacy.generated.h"

USTRUCT(BlueprintType)
struct FTPBufStatInfo
{
	GENERATED_BODY()
public:
	FTPBufStatInfo() :StatType(EBufStatType::BST_NONE), StatValue(0.f)
	{

	}

	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBufStatType StatType;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StatValue;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTTPS_API UTPSkillBase_Legacy : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSkillBase_Legacy();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Buf, Meta = (AllowPrivateAccess = true))
// 	TSubclassOf<UTPSkillBufQueryBase> TargetBufBP;
// 	TObjectPtr<UTPSkillBufQueryBase> CurBufBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Buf)
	TObjectPtr< UTPSkillComponent> OwnCharStatComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Buf, Meta = (AllowprivateAccess = true))
	float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Buf, Meta = (AllowprivateAccess = true))
	float CurCoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Buf, Meta = (AllowprivateAccess = true))
	bool NeedDurationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Buf, Meta = (AllowprivateAccess = true))
	float DurationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Buf, Meta = (AllowprivateAccess = true))
	float CurDurationTime;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetSkill(int InSkillIndex , int InSkillLevel, TObjectPtr< UTPSkillComponent> OwnComp);
	
	virtual void ProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo, TObjectPtr<UTPCharacterStatComponent> InStatComp){}
	virtual void UnProcessBuf(struct FTPCharacterStatInfo& InPrevStatInfo, TObjectPtr<UTPCharacterStatComponent> InStatComp){}
	virtual bool CheckBufTime(float DeltaTime){return true;}

	int SkillIndex;
	int CurSkillLV;
	float CurrentColdTime;
};
