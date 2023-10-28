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

void APCRSerinRightHandCharacter::HandleChase(float DeltaTime)
{
	TargetLocation = CachedSerinCharacter->CachedErica->GetActorLocation() + CachedSerinCharacter->RightHandChaseDistance;
	Super::HandleChase(DeltaTime);
}

void APCRSerinRightHandCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

