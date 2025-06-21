// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPassive_Bullet.h"
#include "../../../TPCharacter.h"
#include "../Buf/TPBufBase.h"



void UTPPassive_Bullet::RunSkill(int EffectIndex)
{
	Super::RunSkill(EffectIndex);

	TPCHECK(CurOwnerStatComp != nullptr);

	// Bullet ¿©·¯¹ß ½î±â.

	//ESkillEffectType
	switch (CurPassiveInfo[EffectIndex]->EffectType)
	{
	case ESkillEffectType::SEffect_ADD_FIRE_BULLET_NUM:
		{
			// ÃÑ¾Ë ¿©·¯°³ ¹ß»ç.
			for (int iShotCount = 0; iShotCount < CurPassiveInfo[EffectIndex]->EffectValue; ++iShotCount)
				CurOwnerStatComp->GetOwnChar()->ShotBullet();
		}
		break;
	case ESkillEffectType::SEffect_ADD_BOUNCE_BULLET:
		{
			// ÃÑ¾Ë ÁÖº¯À¸·Î Æ¨±è.
		}
		break;
	default:
		break;
	}
}
