// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Base/PCRSerinDollBaseCharacter.h"

#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Game/PCRSoundPrimaryDataAsset.h"
#include "Game/PCRParameterDataAsset.h"

APCRSerinDollBaseCharacter::APCRSerinDollBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UPCRSerinDollPrimaryDataAsset> DA_Serin(TEXT("/Script/ProjectCardReturn.PCRSerinDollPrimaryDataAsset'/Game/DataAssets/DA_SerinDoll.DA_SerinDoll'"));
	if (DA_Serin.Succeeded())
	{
		SerinDollDataAsset = DA_Serin.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRSoundPrimaryDataAsset> DA_Sound(TEXT("/Script/ProjectCardReturn.PCRSoundPrimaryDataAsset'/Game/DataAssets/DA_Sound.DA_Sound'"));
	if (DA_Sound.Succeeded())
	{
		SoundDataAsset = DA_Sound.Object;
	}
}

void APCRSerinDollBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(SerinDollDataAsset);
	check(ParameterDataAsset);
	check(SoundDataAsset);
}

void APCRSerinDollBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APCRSerinDollBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APCRSerinDollBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

