// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/RangedSoldier/PCRRangedSoldierAnimInstance.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Soldier/RangedSoldier/PCRRangedSoldierCharacter.h"

DEFINE_LOG_CATEGORY(PCRLogRangedSoldierAnimInstance);

void UPCRRangedSoldierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedRangedSoldier = Cast<APCRRangedSoldierCharacter>(CachedMonsterBaseCharacter);
}

void UPCRRangedSoldierAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPCRRangedSoldierAnimInstance::Throw()
{
	if (!bCanAttack)
	{
		return;
	}
	
	UE_LOG(PCRLogRangedSoldierAnimInstance, Warning, TEXT("Throw!"));
	
	Montage_Play(MonsterDataAsset->RangedSoldierThrowAnimationMontage);

	bCanAttack = false;
}

void UPCRRangedSoldierAnimInstance::AnimNotify_Throw()
{
	check(CachedRangedSoldier);

	CachedRangedSoldier->Throw();
}
