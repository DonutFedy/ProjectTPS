// Fill out your copyright notice in the Description page of Project Settings.


#include "TPActive_HealZone.h"
#include "../Buf/TPBufBase.h"
#include "../../../TPCharacter.h"
#include "../TPSkillBase.h"
#include "../TPSkillController.h"
#include "Obj/TPHealZone.h"
#include "GameFramework/Actor.h"



void UTPActive_HealZone::RunSkill(int EffectIndex)
{
	TPCHECK(CurOwnerStatComp != nullptr);


	//ESkillEffectType
	TObjectPtr< ATPCharacter> CurOwner = CurOwnerStatComp->GetOwnChar();
	{
		// 실제 발동은 이때 하니까...
		Super::RunSkill(EffectIndex);
		// 오브젝트 생성.
		//CurSkillInfo.SkillBP_Path
		TObjectPtr<ATPHealZone> CurHealZone(nullptr);
		if (ArrSkillObj.Num())
		{
			FTPActiveSkillObjState* FoundSkillObj = ArrSkillObj.FindByPredicate(
				[](const FTPActiveSkillObjState& a) { return a.IsUsing; }
			);
			if (FoundSkillObj)
			{
				CurHealZone = Cast<ATPHealZone>(FoundSkillObj->RealObj);
				FoundSkillObj->IsUsing = true;
			}
		}
		if (CurHealZone == nullptr)
		{
			FTPActiveSkillObjState NewSkillObj;
			CurHealZone = CurOwner->GetWorld()->SpawnActor<ATPHealZone>(LoadClass<ATPHealZone>(nullptr, *CurSkillInfo.SkillBP_Path));
			NewSkillObj.RealObj = CurHealZone;
			NewSkillObj.IsUsing = true;
			ArrSkillObj.Add(NewSkillObj);
		}

		if (CurHealZone)
		{
			FRotator OffsetRot = CurOwner->GetActorForwardVector().Rotation();

			CurHealZone->SetActorLocation(CurOwner->GetActorLocation());
			CurHealZone->SetActorRotation(OffsetRot);
			CurHealZone->InitHealZone(this, CurActiveInfo[EffectIndex]->EffectValue, CurActiveInfo[EffectIndex]->EffectDuration, 700, CurOwnerStatComp->GetOwnChar(), CurOwnerStatComp->GetOwnChar()->IsPlayer());
		}

		return;
	}
}
