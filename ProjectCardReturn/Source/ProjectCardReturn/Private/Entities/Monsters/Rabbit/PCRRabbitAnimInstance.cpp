// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/PCRRabbitAnimInstance.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"

DEFINE_LOG_CATEGORY(PCRRabbitAnimInstance);

UPCRRabbitAnimInstance::UPCRRabbitAnimInstance()
{
	bJump = false;
}

void UPCRRabbitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedRabbit = Cast<APCRRabbitCharacter>(CachedMonsterBaseCharacter);
}

void UPCRRabbitAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedRabbit)
	{
		return;
	}

	//bJump = CachedRabbit->IsJumpProvidingForce();
	//bJump = CachedRabbit->bWasJumping;
}

void UPCRRabbitAnimInstance::Attack()
{
	if (!bCanAttack)
	{
		return;
	}

	UE_LOG(PCRRabbitAnimInstance, Warning, TEXT("Attack!"));
	
	Montage_Play(MonsterDataAsset->RabbitAttackAnimationMontage);

	bCanAttack = false;
}

void UPCRRabbitAnimInstance::AnimNotify_Hit()
{
	check(CachedRabbit);
	
	CachedRabbit->Hit();
}
