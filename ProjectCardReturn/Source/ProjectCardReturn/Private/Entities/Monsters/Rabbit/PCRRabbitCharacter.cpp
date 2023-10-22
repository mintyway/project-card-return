// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"
#include "UI/PCRUIDataAsset.h"

#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Entities/Monsters/Rabbit/PCRRabbitAnimInstance.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

APCRRabbitCharacter::APCRRabbitCharacter()
{
	if (ParameterDataAsset)
	{
		MaxHP = ParameterDataAsset->RabbitMaxHealthPoint;
		CurrentHP = MaxHP;
		AttackPower = ParameterDataAsset->RabbitAttackPower;
		AttackRange = ParameterDataAsset->RabbitAttackRange;
		AttackRate = ParameterDataAsset->RabbitAttackRate;
	}

	// TODO: 모델링 작업 완료되면 활성화
	if (GetMesh() && MonsterDataAsset)
	{
		GetMesh()->SetSkeletalMesh(MonsterDataAsset->RabbitMesh);
		
		if (UClass* AnimationBlueprint = MonsterDataAsset->RabbitAnimationBlueprint.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(AnimationBlueprint);
		}
	}

	if (GetCharacterMovement() && ParameterDataAsset)
	{
		GetCharacterMovement()->MaxWalkSpeed = ParameterDataAsset->RabbitMoveSpeed;
	}
}

void APCRRabbitCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RabbitAnimInstance = Cast<UPCRRabbitAnimInstance>(GetMesh()->GetAnimInstance());
	check(RabbitAnimInstance);
}
