// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/Hand/Base/PCRSerinHandBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/Serin/Base/PCRSerinPrimaryDataAsset.h"

APCRSerinHandBaseCharacter::APCRSerinHandBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// TODO: 모델링 구현시 제거
	DummyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyStaticMeshComponent"));
	if (SerinDataAsset)
	{
		DummyStaticMeshComponent->SetupAttachment(GetCapsuleComponent());
		DummyStaticMeshComponent->SetStaticMesh(SerinDataAsset->CubeDummyMesh);
	}
}

void APCRSerinHandBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRSerinHandBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRSerinHandBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

