// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierAnimInstance.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierCharacter.h"

DEFINE_LOG_CATEGORY(PCRLogMeleeSoldierAnimInstance);

UPCRMeleeSoldierAnimInstance::UPCRMeleeSoldierAnimInstance(): bHasShield(false) {}

void UPCRMeleeSoldierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedMeleeSoldier = Cast<APCRMeleeSoldierCharacter>(CachedMonsterBaseCharacter);

	OnMontageEnded.AddDynamic(this, &UPCRMeleeSoldierAnimInstance::AttackMontageEnded);
}

void UPCRMeleeSoldierAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CachedMeleeSoldier)
	{
		bHasShield = CachedMeleeSoldier->GetHasShield();
	}
}

void UPCRMeleeSoldierAnimInstance::Hit()
{
	if (!bCanAttack)
	{
		return;
	}

	UE_LOG(PCRLogMeleeSoldierAnimInstance, Log, TEXT("Attack!"));
	
	if (bHasShield)
	{
		ShieldAttack();
	}
	else
	{
		SpearAttack();
	}

	bCanAttack = false;
}

void UPCRMeleeSoldierAnimInstance::AnimNotify_Hit()
{
	check(CachedMeleeSoldier);

	CachedMeleeSoldier->Hit();
}

void UPCRMeleeSoldierAnimInstance::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MonsterDataAsset->MeleeSoldierShieldAttackAnimationMontage)
	{
		bCanAttack = true;
	}

	if (Montage == MonsterDataAsset->MeleeSoldierSpearAttackAnimationMontage)
	{
		bCanAttack = true;
	}
}

void UPCRMeleeSoldierAnimInstance::ShieldAttack()
{
	if (FMath::RandBool())
	{
		Montage_JumpToSection(TEXT("Part1"));
	}
	else
	{
		Montage_JumpToSection(TEXT("Part2"));
	}
	
	Montage_Play(MonsterDataAsset->MeleeSoldierShieldAttackAnimationMontage);
}

void UPCRMeleeSoldierAnimInstance::SpearAttack()
{
	Montage_Play(MonsterDataAsset->MeleeSoldierSpearAttackAnimationMontage);
}
