// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/Hand/PCRSerinLeftHandCharacter.h"

APCRSerinLeftHandCharacter::APCRSerinLeftHandCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = nullptr;
}

void APCRSerinLeftHandCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRSerinLeftHandCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRSerinLeftHandCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

