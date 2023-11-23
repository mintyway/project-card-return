// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/PCRRabbitAnimInstance.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"

DEFINE_LOG_CATEGORY(PCRRabbitAnimInstance);

UPCRRabbitAnimInstance::UPCRRabbitAnimInstance()
{
	bCanJump = true;
}

void UPCRRabbitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedRabbit = Cast<APCRRabbitCharacter>(CachedMonsterBaseCharacter);

	OnMontageEnded.AddDynamic(this, &UPCRRabbitAnimInstance::AttackMontageEnded);
}

void UPCRRabbitAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UPCRRabbitAnimInstance::Hit()
{
	if (!bCanAttack)
	{
		return;
	}

	UE_LOG(PCRRabbitAnimInstance, Warning, TEXT("Attack!"));
	
	Montage_Play(MonsterDataAsset->RabbitAttackAnimationMontage);

	bCanAttack = false;
}

void UPCRRabbitAnimInstance::Jump()
{
	Montage_Play(MonsterDataAsset->RabbitJumpAnimationMontage);

	CachedRabbit->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);

	bCanJump = false;
}

void UPCRRabbitAnimInstance::AnimNotify_Hit()
{
	check(CachedRabbit);
	
	CachedRabbit->Hit();
}

void UPCRRabbitAnimInstance::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MonsterDataAsset->RabbitAttackAnimationMontage)
	{
		bCanAttack = true;
	}

	if (Montage == MonsterDataAsset->RabbitJumpAnimationMontage)
	{
		CachedRabbit->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
		
		bCanJump = true;
	}
}
