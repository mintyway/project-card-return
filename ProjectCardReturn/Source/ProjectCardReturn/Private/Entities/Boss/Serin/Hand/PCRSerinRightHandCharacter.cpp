// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/Hand/PCRSerinRightHandCharacter.h"

#include "Entities/Boss/Serin/PCRSerinCharacter.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

APCRSerinRightHandCharacter::APCRSerinRightHandCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APCRSerinRightHandCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRSerinRightHandCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRSerinRightHandCharacter::HandleBasicChase(float DeltaTime)
{
	TargetLocation = CachedSerinCharacter->CachedErica->GetActorLocation() + CachedSerinCharacter->RightHandBasicChaseDistance;
	Super::HandleBasicChase(DeltaTime);
}

void APCRSerinRightHandCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

