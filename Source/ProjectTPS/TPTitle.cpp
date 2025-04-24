// Fill out your copyright notice in the Description page of Project Settings.


#include "TPTitle.h"
#include "TPGameInstance.h"

void UTPTitle::StartGame()
{
    auto CurInstance = Cast<UTPGameInstance>( GetGameInstance());

    if (CurInstance)
    {
        CurInstance->StartGame();
    }

}
