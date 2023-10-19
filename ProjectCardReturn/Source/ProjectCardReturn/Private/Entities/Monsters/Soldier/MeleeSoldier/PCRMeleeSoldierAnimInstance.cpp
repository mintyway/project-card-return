// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierAnimInstance.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierCharacter.h"

DEFINE_LOG_CATEGORY(PCRLogMeleeSoldierAnimInstance);

UPCRMeleeSoldierAnimInstance::UPCRMeleeSoldierAnimInstance(): bHasShield(false), bCanAttack(true) {}

void UPCRMeleeSoldierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedMeleeSoldier = Cast<APCRMeleeSoldierCharacter>(CachedMonsterBaseCharacter);
	if (!CachedMeleeSoldier)
	{
		NULL_POINTER_EXCEPTION(CachedMeleeSoldier);
	}

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

void UPCRMeleeSoldierAnimInstance::Attack()
{
	if (!bCanAttack)
	{
		return;
	}

	UE_LOG(PCRLogMeleeSoldierAnimInstance, Warning, TEXT("Attack!"));
	
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

void UPCRMeleeSoldierAnimInstance::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bCanAttack = true;
}

void UPCRMeleeSoldierAnimInstance::ShieldAttack()
{
	Montage_Play(MonsterDataAsset->MeleeSoldierShieldAttackAnimationMontage);
}

void UPCRMeleeSoldierAnimInstance::SpearAttack()
{
	Montage_Play(MonsterDataAsset->MeleeSoldierSpearAttackAnimationMontage);
}
