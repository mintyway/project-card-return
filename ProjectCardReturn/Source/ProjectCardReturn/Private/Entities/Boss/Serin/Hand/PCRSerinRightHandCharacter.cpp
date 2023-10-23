// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/Hand/PCRSerinRightHandCharacter.h"

// Sets default values
APCRSerinRightHandCharacter::APCRSerinRightHandCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = nullptr;
	
}

void APCRSerinRightHandCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRSerinRightHandCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRSerinRightHandCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

