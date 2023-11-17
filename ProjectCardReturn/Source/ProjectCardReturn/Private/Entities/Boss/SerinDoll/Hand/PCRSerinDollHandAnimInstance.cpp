// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandAnimInstance.h"

#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
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

void UPCRSerinDollHandAnimInstance::PlayRockAttack()
{
	Montage_Play(SerinDollDataAsset->RockAttackAnimMontage);
}

void UPCRSerinDollHandAnimInstance::PlayPaperAttack(bool bIsFarAttack)
{
	Montage_Play(SerinDollDataAsset->PaperAttackAnimMontage);
	const FName AttackLocationKey = bIsFarAttack ? TEXT("Back") : TEXT("Front");
	Montage_JumpToSection(AttackLocationKey, SerinDollDataAsset->PaperAttackAnimMontage);
}

void UPCRSerinDollHandAnimInstance::PlayScissorsAttack()
{
	CurrentScissorsAttackCount = 0;
	Montage_Play(SerinDollDataAsset->ScissorsAttackAnimMontage);
}

void UPCRSerinDollHandAnimInstance::PlayPattern1(bool IsLeftHand)
{
	Montage_Play(SerinDollDataAsset->Pattern1AnimMontage);
	const FName HandNameKey = IsLeftHand ? TEXT("LeftAttack") : TEXT("RightAttack");
	Montage_SetNextSection(TEXT("Start"), HandNameKey, SerinDollDataAsset->Pattern1AnimMontage);
	
	Montage_JumpToSection(HandNameKey, SerinDollDataAsset->Pattern1AnimMontage);
}

void UPCRSerinDollHandAnimInstance::AnimNotify_RockAttackChaseEnd()
{
	OnRockAttackEnded.Broadcast();
}

void UPCRSerinDollHandAnimInstance::AnimNotify_RockAttackHit()
{
	OnRockAttackHit.Broadcast();
}

void UPCRSerinDollHandAnimInstance::AnimNotify_PaperAttackSweepStart()
{
	OnPaperAttackSweepStart.Broadcast();
}

void UPCRSerinDollHandAnimInstance::AnimNotify_PaperAttackSweepEnd()
{
	OnPaperAttackSweepEnd.Broadcast();
}

void UPCRSerinDollHandAnimInstance::AnimNotify_ScissorsAttackHitStart()
{
	OnScissorsAttackHitStart.Broadcast();
}

void UPCRSerinDollHandAnimInstance::AnimNotify_ScissorsAttackHitEnd()
{
	OnScissorsAttackHitEnd.Broadcast();
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

void UPCRSerinDollHandAnimInstance::AnimNotify_ScissorsAttackEffectStart()
{
	OnScissorsAttackEffectStart.Broadcast();
}

void UPCRSerinDollHandAnimInstance::AnimNotify_ToIdle()
{
	if (OnToIdle.IsBound())
	{
		OnToIdle.Execute();
	}
}
