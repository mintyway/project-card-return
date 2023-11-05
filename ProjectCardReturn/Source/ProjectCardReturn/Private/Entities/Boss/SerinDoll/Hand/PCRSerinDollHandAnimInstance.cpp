// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandAnimInstance.h"

#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/PCRSerinDollHeadCharacter.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandCharacter.h"
#include "Game/PCRParameterDataAsset.h"

UPCRSerinDollHandAnimInstance::UPCRSerinDollHandAnimInstance()
	: CurrentScissorsAttackCount(0)
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

#if WITH_EDITOR
	if (!GetWorld()->IsPlayInEditor())
	{
		return;
	}
#endif

	check(SerinDollDataAsset && ParameterDataAsset);

	CachedSerinDollHand = Cast<APCRSerinDollHandCharacter>(TryGetPawnOwner());
	check(CachedSerinDollHand);
}

void UPCRSerinDollHandAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UPCRSerinDollHandAnimInstance::PlayRock()
{
	Montage_Play(SerinDollDataAsset->RockAttackAnimMontage);
}

void UPCRSerinDollHandAnimInstance::PlayPaper()
{
	Montage_Play(SerinDollDataAsset->PaperAttackAnimMontage);
}

void UPCRSerinDollHandAnimInstance::PlayScissorsAttack(APCRSerinDollHandCharacter* InSerinDollHand)
{
	CurrentScissorsAttackCount = 0;
	Montage_Play(SerinDollDataAsset->ScissorsAttackAnimMontage);
}

void UPCRSerinDollHandAnimInstance::AnimNotify_ScissorsAttackCountCheck()
{
	++CurrentScissorsAttackCount;
	if (CurrentScissorsAttackCount >= CachedSerinDollHand->GetMaxScissorsAttackCount())
	{
		const UAnimMontage* Montage = SerinDollDataAsset->ScissorsAttackAnimMontage;
		Montage_SetNextSection(TEXT("Attack"), TEXT("End"), Montage);
	}
}

void UPCRSerinDollHandAnimInstance::AnimNotify_ToIdle()
{
	if (OnToIdle.IsBound())
	{
		OnToIdle.Execute();
	}
}
