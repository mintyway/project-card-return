// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandCharacter.h"

DEFINE_LOG_CATEGORY(PCRLogSerinHandCharacter);

APCRSerinDollHandCharacter::APCRSerinDollHandCharacter()
{
	
}

void APCRSerinDollHandCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APCRSerinDollHandCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSerinDollHandCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APCRSerinDollHandCharacter::SetSerinDollHead(APCRSerinDollHeadCharacter* InSerinDollHead)
{
	SerinDollHead = InSerinDollHead;
}
