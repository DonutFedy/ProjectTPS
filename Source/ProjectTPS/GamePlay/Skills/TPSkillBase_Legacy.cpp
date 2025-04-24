// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSkillBase_Legacy.h"

// Sets default values for this component's properties
UTPSkillBase_Legacy::UTPSkillBase_Legacy()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

}


// Called when the game starts
void UTPSkillBase_Legacy::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTPSkillBase_Legacy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTPSkillBase_Legacy::SetSkill(int InSkillIndex, int InSkillLevel, TObjectPtr< UTPSkillComponent> OwnComp)
{
	SkillIndex = InSkillIndex; 
	CurSkillLV = InSkillLevel; 
	OwnCharStatComp = OwnComp;

	//CurBufBP = NewObject<UTPSkillBufQueryBase>(GetTransientPackage(), TargetBufBP);
}

