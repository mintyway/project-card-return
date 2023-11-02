// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollRightHandCharacter.h"

#include "Entities/Boss/SerinDoll/PCRSerinDollCharacter.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

APCRSerinDollRightHandCharacter::APCRSerinDollRightHandCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	if (GetMesh() && SerinDataAsset)
	{
		GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetSkeletalMesh(SerinDataAsset->SerinDollRightHandMesh);
	}
}

void APCRSerinDollRightHandCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSerinDollRightHandCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCRSerinDollRightHandCharacter::PaperChase(bool bUseReset)
{
	Super::PaperChase(bUseReset);

	PaperRotation = FRotator(0, 180.0, 90.0);
}

void APCRSerinDollRightHandCharacter::HandleBasicChase(float DeltaTime)
{
	ChaseLocation = CachedSerinDollCharacter->CachedErica->GetActorLocation();
	ChaseLocation.Y -= CachedSerinDollCharacter->BasicChaseYDistance;
	Super::HandleBasicChase(DeltaTime);
}

void APCRSerinDollRightHandCharacter::HandlePaperChase(float DeltaTime)
{
	ChaseLocation = CachedSerinDollCharacter->CachedErica->GetActorLocation();
	ChaseLocation.Y -= CachedSerinDollCharacter->BasicChaseYDistance + 300.f;
	Super::HandlePaperChase(DeltaTime);
}

void APCRSerinDollRightHandCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
