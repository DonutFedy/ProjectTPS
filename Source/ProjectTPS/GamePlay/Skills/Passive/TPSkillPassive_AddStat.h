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
	
�ǵ�							�ִ�ġ ����
ü��							�ִ�ġ ����
���¹̳�						�ִ�ġ ����
���ݷ�						���� ���ݷ� ����
����						ĳ���� ���� ����
�̵��ӵ�						�̵��ӵ� ����
ġ��Ÿ Ȯ��					ġ��Ÿ Ȯ�� ����
������ �ð�					������ �ð� ����
����ü �ӵ� ����				����ü �ӵ� ����
��Ȯ�� ����					ź���� ���� ����
�ݵ� ����					�ݵ� ��ġ ����
������ �����ð� ����			������ �� �����ð�(��) ����
������ ���¹̳� �Ҹ� ����		"�����⿡ �Ҹ�Ǵ� ���¹̳ʷ� ����"
ź�� ����					�ִ� ź�� ������ ����
�߻� ź�� ����				źâ(�Ű���)�� �ִ뷮 ����
��Ÿ��						�������� ��� ��ų ��Ÿ�� ����
��ų ����					���� ������ ��ų���� ���� ����
�ڱ�							�ڱ� ȹ�� �� ȹ�淮 ����
	*/
};
