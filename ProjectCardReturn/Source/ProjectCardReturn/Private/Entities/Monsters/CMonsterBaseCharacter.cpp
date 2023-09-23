// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/CMonsterBaseCharacter.h"

ACMonsterBaseCharacter::ACMonsterBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACMonsterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACMonsterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACMonsterBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

