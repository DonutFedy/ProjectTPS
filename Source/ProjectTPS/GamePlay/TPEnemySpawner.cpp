// Fill out your copyright notice in the Description page of Project Settings.


#include "TPEnemySpawner.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"

ATPEnemySpawner::ATPEnemySpawner()
{
    CurSpawnerType = ESpawnerType::EST_NONE;
#if WITH_EDITORONLY_DATA
//     DebugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DebugMesh"));
//     DebugMesh->SetupAttachment(RootComponent);
// 
//     // 에디터에서만 보이고, 게임에서는 안 보이게
//     DebugMesh->SetHiddenInGame(true);
//     DebugMesh->SetVisibility(true);
//     DebugMesh->bIsEditorOnly = true; // 핵심 라인!!
#endif
}

FVector ATPEnemySpawner::GetSpawnPoint()
{
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys) return FVector::ZeroVector;

    FNavLocation RandomLocation;
    bool bFound = NavSys->GetRandomReachablePointInRadius(GetActorLocation(), SpawnRange, RandomLocation);

    if (bFound)
        return RandomLocation.Location;
    return GetActorLocation();
}

void ATPEnemySpawner::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

#if WITH_EDITOR
    // 액터 위치에 반지름 100짜리 구 표시
    DrawDebugBox(GetWorld(), GetActorLocation(), FVector(5.f, 5.f, 10.f), FQuat::Identity, FColor::Green, false, -1.f, 0, 2.f);
#endif
}
