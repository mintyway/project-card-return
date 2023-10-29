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
		GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 90.0));
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

void APCRSerinDollRightHandCharacter::HandleBasicChase(float DeltaTime)
{
	ChaseLocation = CachedSerinDollCharacter->CachedErica->GetActorLocation();
	ChaseLocation.Y += CachedSerinDollCharacter->RightHandBasicChaseYDistance;
	Super::HandleBasicChase(DeltaTime);
}

void APCRSerinDollRightHandCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

