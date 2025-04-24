// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


//#include "CoreMinimal.h"
#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ESkillType : uint8 {
	ST_NONE,
	ST_PASSIVE,
	ST_ACTIVE,
};

UENUM(BlueprintType)
enum class ESkillCategoryType : uint8 {
	SCT_NONE,
	SCT_ATTACK,
	SCT_HEALING,
	SCT_UTILITY,
};
UENUM(BlueprintType)
enum class ESkillUseType : uint8 {
	SUT_NONE,
	SUT_ATTACK,
	SUT_HEALING,
	SUT_UTILITY,
};
UENUM(BlueprintType)
enum class ESkillConditionType : uint8 {
	SCondition_NONE,
	SCondition_USE_DODGE,
	SCondition_CHARACTER_FIRE_GUN,
	SCondition_ENEMY_HITTED_BULLET,
	SCondition_CHARACTER_CURRENT_SHIELD,
	SCondition_CHARACTER_CURRENT_HP,
	SCondition_CHARACTER_CURRENT_STAMINA,
	SCondition_CHARACTER_STAT_SHIELDMAX,
	SCondition_CHARACTER_STAT_HPMAX,
	SCondition_CHARACTER_STAT_STAMINAMAX,
	SCondition_CHARACTER_PASSIVE_NUM,
	SCondition_CHARACTER_CURRENT_WEAPON_AR,
	SCondition_CHARACTER_CURRENT_WEAPON_SG,
	SCondition_CHARACTER_CURRENT_WEAPON_SR,
	SCondition_CHARACTER_CURRENT_WEAPON_HG,
};

UENUM(BlueprintType)
enum class ESkillTargetType : uint8 {
	STarget_NONE,
	STarget_TARGET_SELF,
	STarget_TARGET_ENEMY_HITTED,
	STarget_TARGET_ENEMY_DEAD,
	STarget_AREA_CHARACTER,
	STarget_AREA_ENEMY,
};

UENUM(BlueprintType)
enum class ESkillAreaType : uint8 {
	SArea_NONE,
	SArea_CIRCLE,
	SArea_SQUARE,
	SArea_TRIANGLE,
	SArea_DONUT,
	SArea_HALFCIRCLE,
};

UENUM(BlueprintType)
enum class ESkillEffectType : uint8 {
	SEffect_NONE, 
	SEffect_TAKE_DMG,
	SEffect_ADD_WEAPON_ATK,
	SEffect_ADD_CURRENT_HP,
	SEffect_ADD_TEMP_ARMOR,
	SEffect_ADD_CHARACTER_STAT_SHIELDMAX,
	SEffect_ADD_CHARACTER_STAT_SHIELDREST,
	SEffect_ADD_CHARACTER_STAT_SHIELDRECOVER,
	SEffect_ADD_CHARACTER_STAT_HPMAX,
	SEffect_ADD_CHARACTER_STAT_HPREST,
	SEffect_ADD_CHARACTER_STAT_HPRECOVER,
	SEffect_ADD_CHARACTER_STAT_STAMINAMAX,
	SEffect_ADD_CHARACTER_STAT_STAMINAREST,
	SEffect_ADD_CHARACTER_STAT_STAMINARECOVER,
	SEffect_ADD_CHARACTER_STAT_ATK,
	SEffect_ADD_CHARACTER_STAT_SKILLATK,
	SEffect_ADD_CHARACTER_STAT_SKILLCOOLTIME,
	SEffect_ADD_CHARACTER_STAT_DEF,
	SEffect_ADD_CHARACTER_STAT_MOVESPD,
	SEffect_ADD_CHARACTER_STAT_CRITRATE,
	SEffect_ADD_CHARACTER_STAT_RELOAD,
	SEffect_ADD_CHARACTER_STAT_BULLETSPD,
	SEffect_ADD_CHARACTER_STAT_ACCURACY,
	SEffect_ADD_CHARACTER_STAT_RECOIL,
	SEffect_ADD_DODGE_INVINSIBLE,
	SEffect_MULTIPLY_DODGE_STAMINA,
	SEffect_ADD_FIRE_BULLET_NUM,
	SEffect_ADD_BOUNCE_BULLET,
	SEffect_MULTIPLY_WEAPON_STAT_MAXAMMO,
	SEffect_ADD_ACTIVE_STAT_USECOUNT,
	SEffect_ADD_WEAPON_AR_STAT_ATK,
	SEffect_ADD_WEAPON_SG_STAT_ATK,
	SEffect_ADD_WEAPON_SR_STAT_ATK,
	SEffect_ADD_WEAPON_HG_STAT_ATK,
	SEffect_ADD_WEAPON_AR_STAT_FIRERATE,
	SEffect_ADD_WEAPON_SG_STAT_FIRERATE,
	SEffect_ADD_WEAPON_SR_STAT_FIRERATE,
	SEffect_ADD_WEAPON_HG_STAT_FIRERATE,
};

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	PREINIT,
	LOADING,
	READY,
	DEAD,
};


UENUM(BlueprintType)
enum class EBufType : uint8 {
	BT_NONE,
	BT_ADD_STAT,
	BT_MUL_STAT,
};

UENUM(BlueprintType)
enum class EBufStatType : uint8 {
	BST_NONE,
	BST_ATTACK,
	BST_ATTACK_RATE,
	BST_ATTACK_RANGE,
	BST_WEAPON_ATTACK,
	BST_SKILL_ATTACK,
	BST_DEFENCE,
	BST_DEFENCE_RATE,
	BST_PIERCE,
	BST_CRITICAL_RATE,
	BST_CRITICAL_DAMAGE,
	BST_CRITICAL_DAMAGE_RATE,
	BST_SHIELD_MAX,
	BST_SHIELD_REST,
	BST_SHIELD_RECOVER,
	BST_HEALTH_MAX, // 최대치
	BST_HEALTH_REST,
	BST_HEALTH_RECOVER,
	BST_STAMINA_MAX,
	BST_STAMINA_REST,
	BST_STAMINA_RECOVER,
	BST_MOVE_SPD,
	BST_BULLET_SPD,
	BST_FIRE_RATE,
	BST_RELOAD_SPD,
	BST_ACCURACY,
	BST_RECOIL, // 반동 수치
	BST_RECOIL_DOWN, // 반동 수치 감소
	BST_DODGE_INVINCIBILITY_TIME,	//구르기 무적시간 증가
	BST_DODGE_STAMINA_DOWN, // 구르기 스태미너 소모 감소
	BST_INCREASED_BULLET_AMMO, // 최대 탄약 보유량 증가
	BST_INCREASED_BULLET_CHARGE, // 탄창(매거진)의 최대량 증가
	BST_SKILL_COOLTIME,
	BST_DECREASED_SKILL_COOLTIME, // 착용중인 모든 스킬 쿨타임 감소
	BST_INCREASED_SKILL_STACK_COUNT, // 스택 가능한 스킬들의 수량 증가
	BST_INCREASED_REWARD_RATE, // 자금 획득 시 획득량 증가
	BST_DECREASE_ENEMY_STAT_RATE, // 적군 스탯 감소
	BST_JUMP_FORCE,
	BST_DROP_EXP,
	BST_DROP_GROUP,
	BST_MAX,
};

UENUM(BlueprintType)
enum class EAnimAttackType : uint8 {
	NONE,
	NEAR_ATTACK,
	PISTOL,
	RIFLE,
};

UENUM(BlueprintType)
enum class ESpawnerType : uint8
{
	EST_NONE,
	EST_ENEMY,
	EST_GATE,
};

DECLARE_MULTICAST_DELEGATE(FOnNoParamDeletage);

DECLARE_LOG_CATEGORY_EXTERN(ProjectTPS, Log, All);
#define TPLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define TPLOG_S(Verbosity) UE_LOG(ProjectTPS, Verbosity, TEXT("%s"), *TPLOG_CALLINFO)
#define TPLOG(Verbosity, Format, ...) UE_LOG(ProjectTPS, Verbosity, TEXT("%s %s"), *TPLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define TPCHECK(Expr, ...) {if(!(Expr)){ TPLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}

