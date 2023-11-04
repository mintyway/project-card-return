// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandAnimInstance.h"

#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Game/PCRParameterDataAsset.h"

UPCRSerinDollHandAnimInstance::UPCRSerinDollHandAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UPCRSerinDollPrimaryDataAsset> DA_SerinDoll(TEXT("/Script/ProjectCardReturn.PCRSerinDollPrimaryDataAsset'/Game/DataAssets/DA_SerinDoll.DA_SerinDoll'"));
	if (DA_SerinDoll.Succeeded())
	{
		SerinDollDataAsset = DA_SerinDoll.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}
}

void UPCRSerinDollHandAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	check(SerinDollDataAsset && ParameterDataAsset);
}

void UPCRSerinDollHandAnimInstance::PlayRock()
{
	Montage_Play(SerinDollDataAsset->RockAttackAnimMontage);
}
void UPCRSerinDollHandAnimInstance::PlayPaper()
{
	Montage_Play(SerinDollDataAsset->PaperAttackAnimMontage);
}

void UPCRSerinDollHandAnimInstance::PlayScissors()
{
	Montage_Play(SerinDollDataAsset->ScissorsAttackAnimMontage);
}
