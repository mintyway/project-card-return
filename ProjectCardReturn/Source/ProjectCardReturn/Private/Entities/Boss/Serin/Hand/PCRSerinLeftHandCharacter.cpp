// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/Hand/PCRSerinLeftHandCharacter.h"

#include "Entities/Boss/Serin/PCRSerinCharacter.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

APCRSerinLeftHandCharacter::APCRSerinLeftHandCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APCRSerinLeftHandCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSerinLeftHandCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCRSerinLeftHandCharacter::HandleChase(float DeltaTime)
{
	TargetLocation = CachedSerinCharacter->CachedErica->GetActorLocation() + CachedSerinCharacter->LeftHandChaseDistance;
	Super::HandleChase(DeltaTime);
}

void APCRSerinLeftHandCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
