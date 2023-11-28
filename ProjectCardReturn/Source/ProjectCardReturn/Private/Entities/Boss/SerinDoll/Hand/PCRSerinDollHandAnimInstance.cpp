// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandAnimInstance.h"

#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
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

void UPCRSerinDollHandAnimInstance::PlayStart()
{
	Montage_Play(SerinDollDataAsset->HandStartAnimMontage);
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
	Montage_Play(SerinDollDataAsset->HandPattern1AnimMontage);
	const FName HandNameKey = IsLeftHand ? TEXT("LeftAttack") : TEXT("RightAttack");
	Montage_SetNextSection(TEXT("Start"), HandNameKey, SerinDollDataAsset->HandPattern1AnimMontage);

	FOnMontageEnded Pattern1MontageEnded;
	Pattern1MontageEnded.BindUObject(this, &UPCRSerinDollHandAnimInstance::HandlePattern1Ended);
	Montage_SetEndDelegate(Pattern1MontageEnded, SerinDollDataAsset->HandPattern1AnimMontage);
}

void UPCRSerinDollHandAnimInstance::EndPattern1()
{
	if (Montage_IsPlaying(SerinDollDataAsset->HandPattern1AnimMontage))
	{
		const FName EndKey = TEXT("End");
		Montage_JumpToSection(EndKey, SerinDollDataAsset->HandPattern1AnimMontage);
	}
}

void UPCRSerinDollHandAnimInstance::PlayPattern2()
{
	Montage_Play(SerinDollDataAsset->HandPattern2AnimMontage);
}

void UPCRSerinDollHandAnimInstance::HitPattern2()
{
	if (Montage_IsPlaying(SerinDollDataAsset->HandPattern2AnimMontage))
	{
		const FName CurrentSection = Montage_GetCurrentSection(SerinDollDataAsset->HandPattern2AnimMontage);
		if (CurrentSection != TEXT("Hit"))
		{
			Montage_JumpToSection(TEXT("Hit"), SerinDollDataAsset->HandPattern2AnimMontage);
		}
	}
}

void UPCRSerinDollHandAnimInstance::EndPatter2()
{
	if (Montage_IsPlaying(SerinDollDataAsset->HandPattern2AnimMontage))
	{
		Montage_Stop(0.25f, SerinDollDataAsset->HandPattern2AnimMontage);
	}
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

void UPCRSerinDollHandAnimInstance::AnimNotify_Pattern1Shoot()
{
	OnPattern1Shoot.Broadcast();
}

void UPCRSerinDollHandAnimInstance::AnimNotify_ToIdle()
{
	if (OnToIdle.IsBound())
	{
		OnToIdle.Execute();
	}
}

void UPCRSerinDollHandAnimInstance::AnimNotify_Pattern1LastShoot()
{
	OnPattern1LastShoot.Broadcast();
}

void UPCRSerinDollHandAnimInstance::AnimNotify_Pattern2CollisionActivate()
{
	OnPattern2CollisionActivate.Broadcast();
}

void UPCRSerinDollHandAnimInstance::HandlePattern1Ended(UAnimMontage* AnimMontage, bool bArg)
{
	OnPattern1Ended.Broadcast();
}
