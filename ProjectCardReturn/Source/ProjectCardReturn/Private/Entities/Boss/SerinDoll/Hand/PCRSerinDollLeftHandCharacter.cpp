// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollLeftHandCharacter.h"

#include "Entities/Boss/SerinDoll/PCRSerinDollCharacter.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

APCRSerinDollLeftHandCharacter::APCRSerinDollLeftHandCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	if (GetMesh() && SerinDataAsset)
	{
		GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 90.0));
		GetMesh()->SetSkeletalMesh(SerinDataAsset->SerinDollLeftHandMesh);
	}
}

void APCRSerinDollLeftHandCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSerinDollLeftHandCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCRSerinDollLeftHandCharacter::PaperChase(bool bUseReset)
{
	Super::PaperChase(bUseReset);
	
	PaperRotation = FRotator(0, 180.0, -90.0);
}

void APCRSerinDollLeftHandCharacter::HandleBasicChase(float DeltaTime)
{
	ChaseLocation = CachedSerinDollCharacter->CachedErica->GetActorLocation();
	ChaseLocation.Y += CachedSerinDollCharacter->BasicChaseYDistance;
	Super::HandleBasicChase(DeltaTime);
}

void APCRSerinDollLeftHandCharacter::HandlePaperChase(float DeltaTime)
{
	ChaseLocation = CachedSerinDollCharacter->CachedErica->GetActorLocation();
	ChaseLocation.Y += CachedSerinDollCharacter->BasicChaseYDistance + 300.f;
	Super::HandlePaperChase(DeltaTime);
}


void APCRSerinDollLeftHandCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
