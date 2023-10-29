// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Base/PCRSerinBaseCharacter.h"

#include "Entities/Boss/SerinDoll/Base/PCRSerinPrimaryDataAsset.h"
#include "Game/PCRParameterDataAsset.h"

APCRSerinBaseCharacter::APCRSerinBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UPCRSerinPrimaryDataAsset> DA_Serin(TEXT("/Script/ProjectCardReturn.PCRSerinPrimaryDataAsset'/Game/DataAssets/DA_Serin.DA_Serin'"));
	if (DA_Serin.Succeeded())
	{
		SerinDataAsset = DA_Serin.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}
}

void APCRSerinBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRSerinBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRSerinBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

