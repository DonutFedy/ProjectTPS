// Fill out your copyright notice in the Description page of Project Settings.


#include "TPActive_Granade.h"
#include "../Buf/TPBufBase.h"
#include "../../../TPCharacter.h"
#include "../TPSkillBase.h"
#include "../TPSkillController.h"
#include "Obj/TPGranade.h"
#include "GameFramework/Actor.h"



void UTPActive_Granade::RunSkill(int EffectIndex)
{
	TPCHECK(CurOwnerStatComp != nullptr);


	//ESkillEffectType
	switch (CurActiveInfo[EffectIndex]->EffectType)
	{
	case ESkillEffectType::SEffect_TAKE_DMG:
		{
			TObjectPtr< ATPCharacter> CurOwner = CurOwnerStatComp->GetOwnChar();
			// 레디상태로 변경(경로 시각화)
			if (IsNeedToReady() && IsReadyToUse == false)
			{
				SkillReadyAngleOffset = 30.f;
				IsReadyToUse = true;
				return CurOwner->ShowActiveSkillSpline(60, 1000, SkillReadyAngleOffset);
			}
			// 실제 발동
			else
			{
				// 실제 발동은 이때 하니까...
				Super::RunSkill(EffectIndex);
				// 오브젝트 생성.
				//CurSkillInfo.SkillBP_Path
				TObjectPtr<ATPGranade> CurGranade(nullptr);
				if (ArrSkillObj.Num())
				{
					FTPActiveSkillObjState* FoundSkillObj = ArrSkillObj.FindByPredicate(
						[](const FTPActiveSkillObjState& a) { return a.IsUsing; }
					);
					if (FoundSkillObj)
					{
						CurGranade = Cast<ATPGranade>(FoundSkillObj->RealObj);
						FoundSkillObj->IsUsing = true;
					}
				}
				if (CurGranade == nullptr)
				{
					FTPActiveSkillObjState NewSkillObj;
					CurGranade = CurOwner->GetWorld()->SpawnActor<ATPGranade>(LoadClass<ATPGranade>(nullptr, *CurSkillInfo.SkillBP_Path));
					NewSkillObj.RealObj = CurGranade;
					NewSkillObj.IsUsing = true;
					ArrSkillObj.Add(NewSkillObj);
				}

				if (CurGranade)
				{
					FRotator OffsetRot = CurOwner->GetActorForwardVector().Rotation();
					OffsetRot.Pitch += SkillReadyAngleOffset;

					FVector Offset(0, 0, 40);
					CurGranade->SetActorLocation(CurOwner->GetActorLocation() + Offset);
					CurGranade->SetActorRotation(OffsetRot);
					CurGranade->InitGranade(this, CurActiveInfo[EffectIndex]->EffectValue, 1000, 700, CurOwnerStatComp->GetOwnChar(), CurOwnerStatComp->GetOwnChar()->IsPlayer());
				}

				return;
			}
		}
		break;
	default:
		break;
	}
}
