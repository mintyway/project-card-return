// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/PCRSerinCharacter.h"

#include "Entities/Boss/Serin/PCRSerinAIController.h"

APCRSerinCharacter::APCRSerinCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = APCRSerinAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void APCRSerinCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRSerinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRSerinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

