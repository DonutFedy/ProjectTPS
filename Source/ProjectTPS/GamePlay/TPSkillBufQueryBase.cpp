// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSkillBufQueryBase.h"
#include "../Management/TPStageManager.h"
#include "TPSkillBufQuery_AddStat.h"
#include "TPSkillBufQuery_MulStat.h"

// UTPSkillBufQueryBase* UTPSkillBufQueryBase::BufQueryFactory(FTPSkillInfo InFromSkillInfo)
// {
// 	UTPSkillBufQueryBase* NewBufQuery = nullptr;
// 
// // 	EBufType NewBufType = InFromSkillInfo.BufType;
// // 
// // 
// // 	switch (NewBufType)
// // 	{
// // 	case EBufType::BT_ADD_STAT:
// // 	{
// // 		UTPSkillBufQuery_AddStat* CurBuf = NewObject<UTPSkillBufQuery_AddStat>();
// // 		CurBuf->SetBufQuery(InFromSkillInfo.BufStatType, InFromSkillInfo.BufValue);
// // 
// // 		NewBufQuery = CurBuf;
// // 	}
// // 		break;
// // 	case EBufType::BT_MUL_STAT:
// // 	{
// // 		UTPSkillBufQuery_MulStat* CurBuf = NewObject<UTPSkillBufQuery_MulStat>();
// // 		CurBuf->SetBufQuery(InFromSkillInfo.BufStatType, InFromSkillInfo.BufValue);
// // 		NewBufQuery = CurBuf;
// // 	}
// // 		break;
// // 	default:
//		break;
// // 	}
// 
// 
// 	return NewBufQuery;
// }
