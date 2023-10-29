// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinLeftHandCharacter.h"

#include "Entities/Boss/SerinDoll/PCRSerinCharacter.h"
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

void APCRSerinLeftHandCharacter::HandleBasicChase(float DeltaTime)
{
	ChaseLocation = CachedSerinCharacter->CachedErica->GetActorLocation();
	ChaseLocation.Y = CachedSerinCharacter->LeftHandBasicChaseYDistance;
	Super::HandleBasicChase(DeltaTime);
}

void APCRSerinLeftHandCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
