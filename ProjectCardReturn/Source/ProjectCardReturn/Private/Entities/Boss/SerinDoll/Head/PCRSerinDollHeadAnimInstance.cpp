// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadAnimInstance.h"

#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Game/PCRParameterDataAsset.h"

class UPCRParameterDataAsset;
class UPCRSerinDollPrimaryDataAsset;

UPCRSerinDollHeadAnimInstance::UPCRSerinDollHeadAnimInstance()
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

void UPCRSerinDollHeadAnimInstance::PlayPattern1()
{
	Montage_Play(SerinDollDataAsset->HeadPattern1AnimMontage);
}

void UPCRSerinDollHeadAnimInstance::EndPattern1()
{
	if (Montage_IsPlaying(SerinDollDataAsset->HeadPattern1AnimMontage))
	{
		Montage_JumpToSection(TEXT("End"), SerinDollDataAsset->HeadPattern1AnimMontage);
	}
}
