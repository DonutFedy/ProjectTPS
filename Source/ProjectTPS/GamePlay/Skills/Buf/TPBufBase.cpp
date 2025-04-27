// Fill out your copyright notice in the Description page of Project Settings.


#include "TPBufBase.h"
#include "../../../TPSkillComponent.h"

void UTPBufBase::InitBuf(FTPPassiveGroupTable& CurPassiveInfo, TObjectPtr<UTPSkillComponent> InSkillComp)
{
	SkillComp = InSkillComp;
	AddEffect(CurPassiveInfo);
}

bool UTPBufBase::AddEffect(FTPPassiveGroupTable& CurPassiveInfo)
{
	FTPEffectInfo NewEffect;
	NewEffect._BufDuration = CurPassiveInfo.EffectDuration;
	NewEffect.StatValue = CurPassiveInfo.EffectValue;
	switch (CurPassiveInfo.EffectType)
	{
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_SHIELDMAX:
		NewEffect.StatType = EBufStatType::BST_SHIELD_MAX;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_HPMAX:
		NewEffect.StatType = EBufStatType::BST_HEALTH_MAX;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_STAMINAMAX:
		NewEffect.StatType = EBufStatType::BST_STAMINA_MAX;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_ATK:
		NewEffect.StatType = EBufStatType::BST_ATTACK;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_DEF:
		NewEffect.StatType = EBufStatType::BST_DEFENCE;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_MOVESPD:
		NewEffect.StatType = EBufStatType::BST_MOVE_SPD;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_CRITRATE:
		NewEffect.StatType = EBufStatType::BST_CRITICAL_RATE;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_RELOAD:
		NewEffect.StatType = EBufStatType::BST_RELOAD_SPD;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_BULLETSPD:
		NewEffect.StatType = EBufStatType::BST_BULLET_SPD;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_ACCURACY:
		NewEffect.StatType = EBufStatType::BST_ACCURACY;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_RECOIL:
		NewEffect.StatType = EBufStatType::BST_RECOIL;
		break;
	case ESkillEffectType::SEffect_ADD_DODGE_INVINSIBLE:
		NewEffect.StatType = EBufStatType::BST_DODGE_INVINCIBILITY_TIME;
		break;
	case ESkillEffectType::SEffect_MULTIPLY_DODGE_STAMINA:
		NewEffect.StatType = EBufStatType::BST_DODGE_STAMINA_DOWN;
		break;
	case ESkillEffectType::SEffect_MULTIPLY_WEAPON_STAT_MAXAMMO:
		NewEffect.StatType = EBufStatType::BST_INCREASED_BULLET_AMMO;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_SKILLCOOLTIME:
		NewEffect.StatType = EBufStatType::BST_DECREASED_SKILL_COOLTIME;
		break;
	case ESkillEffectType::SEffect_ADD_ACTIVE_STAT_USECOUNT:
		NewEffect.StatType = EBufStatType::BST_INCREASED_SKILL_STACK_COUNT;
		break;
	case ESkillEffectType::SEffect_ADD_FIRE_BULLET_NUM:
	case ESkillEffectType::SEffect_ADD_BOUNCE_BULLET:
		break;
		// 			SEffect_TAKE_DMG,
		// 			SEffect_ADD_WEAPON_ATK,
		// 			SEffect_ADD_CURRENT_HP,
		// 			SEffect_ADD_TEMP_ARMOR,
		// 			SEffect_ADD_CHARACTER_STAT_SHIELDREST,
		// 			SEffect_ADD_CHARACTER_STAT_SHIELDRECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_HPREST,
		// 			SEffect_ADD_CHARACTER_STAT_HPRECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_STAMINAREST,
		// 			SEffect_ADD_CHARACTER_STAT_STAMINARECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_SKILLATK,
		// 			SEffect_ADD_WEAPON_AR_STAT_ATK,
		// 			SEffect_ADD_WEAPON_SG_STAT_ATK,
		// 			SEffect_ADD_WEAPON_SR_STAT_ATK,
		// 			SEffect_ADD_WEAPON_HG_STAT_ATK,
		// 			SEffect_ADD_WEAPON_AR_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_SG_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_SR_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_HG_STAT_FIRERATE,
	default:
		break;
	}
	if(BufStatInfo.Find(NewEffect.StatType) != nullptr)
		return false;


	ProcessBuf(NewEffect, SkillComp->GetCharStatComp());
	BufStatInfo.Add(NewEffect.StatType, NewEffect);
	return true;
}

void UTPBufBase::CheckBufTime(float DeltaTime)
{
	// 각 버프별로 남은 시간을 계산한다.
	// duration이 -1 이면 무한지속이다.
// 	TArray<int> ArrRemoveIndex;
// 	for(int CheckEffectIndex =0; CheckEffectIndex < BufStatInfo.Num(); ++CheckEffectIndex)
// 	{
// 		if(BufStatInfo[CheckEffectIndex].BufDuration == -1)
// 			continue;
// 		BufStatInfo[CheckEffectIndex].BufDuration -= DeltaTime;
// 
// 		if (BufStatInfo[CheckEffectIndex].BufDuration <= 0.f)
// 		{
// 			ArrRemoveIndex.Add(CheckEffectIndex); 
// 		}
// 	}
// 
// 	ArrRemoveIndex.Sort([](int x, int y ){ return x> y;});
// 	for (auto CurRemoveIndex : ArrRemoveIndex)
// 	{
// 		UnProcessBuf(BufStatInfo[CurRemoveIndex], SkillComp->GetCharStatComp());
// 		BufStatInfo.RemoveAt(CurRemoveIndex);
// 	}
}


bool UTPBufBase::RemoveEffect(ESkillEffectType InRemoveTargetType)
{
	int FindTargetIndex = INDEX_NONE;
	EBufStatType FindType = EBufStatType::BST_NONE;
	switch (InRemoveTargetType)
	{
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_SHIELDMAX:
		FindType = EBufStatType::BST_SHIELD_MAX;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_HPMAX:
		FindType = EBufStatType::BST_HEALTH_MAX;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_STAMINAMAX:
		FindType = EBufStatType::BST_STAMINA_MAX;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_ATK:
		FindType = EBufStatType::BST_ATTACK;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_DEF:
		FindType = EBufStatType::BST_DEFENCE;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_MOVESPD:
		FindType = EBufStatType::BST_MOVE_SPD;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_CRITRATE:
		FindType = EBufStatType::BST_CRITICAL_RATE;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_RELOAD:
		FindType = EBufStatType::BST_RELOAD_SPD;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_BULLETSPD:
		FindType = EBufStatType::BST_BULLET_SPD;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_ACCURACY:
		FindType = EBufStatType::BST_ACCURACY;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_RECOIL:
		FindType = EBufStatType::BST_RECOIL;
		break;
	case ESkillEffectType::SEffect_ADD_DODGE_INVINSIBLE:
		FindType = EBufStatType::BST_DODGE_INVINCIBILITY_TIME;
		break;
	case ESkillEffectType::SEffect_MULTIPLY_DODGE_STAMINA:
		FindType = EBufStatType::BST_DODGE_STAMINA_DOWN;
		break;
	case ESkillEffectType::SEffect_MULTIPLY_WEAPON_STAT_MAXAMMO:
		FindType = EBufStatType::BST_INCREASED_BULLET_AMMO;
		break;
	case ESkillEffectType::SEffect_ADD_CHARACTER_STAT_SKILLCOOLTIME:
		FindType = EBufStatType::BST_DECREASED_SKILL_COOLTIME;
		break;
	case ESkillEffectType::SEffect_ADD_ACTIVE_STAT_USECOUNT:
		FindType = EBufStatType::BST_INCREASED_SKILL_STACK_COUNT;
		break;
// 	case ESkillEffectType::SEffect_ADD_FIRE_BULLET_NUM:
// 	case ESkillEffectType::SEffect_ADD_BOUNCE_BULLET:
// 		break;
		// 			SEffect_TAKE_DMG,
		// 			SEffect_ADD_WEAPON_ATK,
		// 			SEffect_ADD_CURRENT_HP,
		// 			SEffect_ADD_TEMP_ARMOR,
		// 			SEffect_ADD_CHARACTER_STAT_SHIELDREST,
		// 			SEffect_ADD_CHARACTER_STAT_SHIELDRECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_HPREST,
		// 			SEffect_ADD_CHARACTER_STAT_HPRECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_STAMINAREST,
		// 			SEffect_ADD_CHARACTER_STAT_STAMINARECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_SKILLATK,
		// 			SEffect_ADD_WEAPON_AR_STAT_ATK,
		// 			SEffect_ADD_WEAPON_SG_STAT_ATK,
		// 			SEffect_ADD_WEAPON_SR_STAT_ATK,
		// 			SEffect_ADD_WEAPON_HG_STAT_ATK,
		// 			SEffect_ADD_WEAPON_AR_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_SG_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_SR_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_HG_STAT_FIRERATE,
	default:
	{
		if (BufStatInfo.Num() <= 0)
			return true;
		return false;
	}
	}

	FTPEffectInfo* FindEffect = BufStatInfo.Find(FindType);

	if (FindEffect != nullptr)
	{
		UnProcessBuf(*FindEffect, SkillComp->GetCharStatComp());
		BufStatInfo.Remove(FindType);

		if(BufStatInfo.Num() <= 0)
			return true;
	}

	return false;
}

void UTPBufBase::ProcessBuf(FTPEffectInfo& NewEffect, TObjectPtr<UTPCharacterStatComponent> InStatComp)
{
	FTPCharacterStatInfo& CurAddStat = InStatComp->GetStatInfo();

	NewEffect.RealApplyValue = NewEffect.StatValue;
	bool NeedMultiply = false;
	switch (NewEffect.StatType)
	{
	case EBufStatType::BST_ATTACK:
	case EBufStatType::BST_DEFENCE:
	case EBufStatType::BST_MOVE_SPD:
	case EBufStatType::BST_CRITICAL_RATE:
	case EBufStatType::BST_RELOAD_SPD:
	case EBufStatType::BST_BULLET_SPD:
	case EBufStatType::BST_ACCURACY:
	case EBufStatType::BST_RECOIL:
	case EBufStatType::BST_DODGE_INVINCIBILITY_TIME:
	case EBufStatType::BST_DODGE_STAMINA_DOWN:
	case EBufStatType::BST_INCREASED_BULLET_AMMO:
	case EBufStatType::BST_DECREASED_SKILL_COOLTIME:
		NeedMultiply = true;
		//NewEffect.RealApplyValue = InPrevStatInfo.GetAddStat(NewEffect.StatType) * NewEffect.StatValue;
		break;
		// 			SEffect_TAKE_DMG,
		// 			SEffect_ADD_WEAPON_ATK,
		// 			SEffect_ADD_CURRENT_HP,
		// 			SEffect_ADD_TEMP_ARMOR,
		// 			SEffect_ADD_CHARACTER_STAT_SHIELDREST,
		// 			SEffect_ADD_CHARACTER_STAT_SHIELDRECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_HPREST,
		// 			SEffect_ADD_CHARACTER_STAT_HPRECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_STAMINAREST,
		// 			SEffect_ADD_CHARACTER_STAT_STAMINARECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_SKILLATK,
		// 			SEffect_ADD_WEAPON_AR_STAT_ATK,
		// 			SEffect_ADD_WEAPON_SG_STAT_ATK,
		// 			SEffect_ADD_WEAPON_SR_STAT_ATK,
		// 			SEffect_ADD_WEAPON_HG_STAT_ATK,
		// 			SEffect_ADD_WEAPON_AR_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_SG_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_SR_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_HG_STAT_FIRERATE,
	default:
		break;
	}
	
	if(NeedMultiply)
		CurAddStat.Mul_AddStat(NewEffect.StatType, NewEffect.RealApplyValue);
	else
		CurAddStat.Add_AddStat(NewEffect.StatType, NewEffect.RealApplyValue);
	InStatComp->CallProcessBufStat(NewEffect.StatType);
}

void UTPBufBase::UnProcessBuf(FTPEffectInfo& NewEffect, TObjectPtr<UTPCharacterStatComponent> InStatComp)
{
	FTPCharacterStatInfo& CurAddStat = InStatComp->GetStatInfo();
	bool NeedMultiply = false;
	switch (NewEffect.StatType)
	{
	case EBufStatType::BST_ATTACK:
	case EBufStatType::BST_DEFENCE:
	case EBufStatType::BST_MOVE_SPD:
	case EBufStatType::BST_CRITICAL_RATE:
	case EBufStatType::BST_RELOAD_SPD:
	case EBufStatType::BST_BULLET_SPD:
	case EBufStatType::BST_ACCURACY:
	case EBufStatType::BST_RECOIL:
	case EBufStatType::BST_DODGE_INVINCIBILITY_TIME:
	case EBufStatType::BST_DODGE_STAMINA_DOWN:
	case EBufStatType::BST_INCREASED_BULLET_AMMO:
	case EBufStatType::BST_DECREASED_SKILL_COOLTIME:
		NeedMultiply = true;
		break;
		// 			SEffect_TAKE_DMG,
		// 			SEffect_ADD_WEAPON_ATK,
		// 			SEffect_ADD_CURRENT_HP,
		// 			SEffect_ADD_TEMP_ARMOR,
		// 			SEffect_ADD_CHARACTER_STAT_SHIELDREST,
		// 			SEffect_ADD_CHARACTER_STAT_SHIELDRECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_HPREST,
		// 			SEffect_ADD_CHARACTER_STAT_HPRECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_STAMINAREST,
		// 			SEffect_ADD_CHARACTER_STAT_STAMINARECOVER,
		// 			SEffect_ADD_CHARACTER_STAT_SKILLATK,
		// 			SEffect_ADD_WEAPON_AR_STAT_ATK,
		// 			SEffect_ADD_WEAPON_SG_STAT_ATK,
		// 			SEffect_ADD_WEAPON_SR_STAT_ATK,
		// 			SEffect_ADD_WEAPON_HG_STAT_ATK,
		// 			SEffect_ADD_WEAPON_AR_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_SG_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_SR_STAT_FIRERATE,
		// 			SEffect_ADD_WEAPON_HG_STAT_FIRERATE,
	default:
		break;
	}

	if (NeedMultiply)
		CurAddStat.Mul_AddStat(NewEffect.StatType, 1.f/NewEffect.RealApplyValue);
	else
		CurAddStat.Add_AddStat(NewEffect.StatType, -NewEffect.RealApplyValue);
	InStatComp->CallProcessBufStat(NewEffect.StatType);
}
