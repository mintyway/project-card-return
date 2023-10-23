// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/PCRSerinCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/Serin/PCRSerinAIController.h"
#include "Entities/Boss/Serin/Base/PCRSerinPrimaryDataAsset.h"
#include "Entities/Boss/Serin/Hand/PCRSerinLeftHandCharacter.h"
#include "Entities/Boss/Serin/Hand/PCRSerinRightHandCharacter.h"

APCRSerinCharacter::APCRSerinCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AIControllerClass = APCRSerinAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DummyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyMeshComponent"));
	if (DummyMeshComponent && SerinDataAsset)
	{
		DummyMeshComponent->SetupAttachment(GetCapsuleComponent());
		DummyMeshComponent->SetStaticMesh(SerinDataAsset->SerinDummyMesh);
	}
}

void APCRSerinCharacter::BeginPlay()
{
	Super::BeginPlay();

	LeftHand = GetWorld()->SpawnActor<APCRSerinLeftHandCharacter>(APCRSerinLeftHandCharacter::StaticClass());
	RightHand = GetWorld()->SpawnActor<APCRSerinRightHandCharacter>(APCRSerinRightHandCharacter::StaticClass());
}

void APCRSerinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRSerinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

