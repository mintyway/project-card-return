// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"
#include "UI/PCRUIDataAsset.h"

#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Entities/Monsters/Rabbit/PCRRabbitAIController.h"
#include "Entities/Monsters/Rabbit/PCRRabbitAnimInstance.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

APCRRabbitCharacter::APCRRabbitCharacter()
{
	if (ParameterDataAsset)
	{
		MaxHP = ParameterDataAsset->RabbitMaxHP;
		CurrentHP = MaxHP;
		MoveSpeed = ParameterDataAsset->RabbitMoveSpeed;
		AttackPower = ParameterDataAsset->RabbitAttackPower;
		AttackRange = ParameterDataAsset->RabbitAttackRange;
		
		GimmickMoveRange = ParameterDataAsset->RabbitGimmickMoveRange;
		JumpProbability = ParameterDataAsset->RabbitJumpProbability;
		WaitAndJumpProbability = ParameterDataAsset->RabbitWaitAndJumpProbability;
		MoveRightDiagonalProbability = ParameterDataAsset->RabbitMoveRightDiagonalProbability;
		MoveLeftDiagonalProbability = ParameterDataAsset->RabbitMoveLeftDiagonalProbability;
		MoveDiagonalDistance = ParameterDataAsset->RabbitMoveDiagonalDistance;
	}

	AIControllerClass = APCRRabbitAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (GetMesh() && MonsterDataAsset)
	{
		GetMesh()->SetSkeletalMesh(MonsterDataAsset->RabbitMesh);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -86.5), FRotator(0.0, -90.0, 0.0));
		
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

	AnimInstance = Cast<UPCRRabbitAnimInstance>(GetMesh()->GetAnimInstance());
	check(AnimInstance);
}

void APCRRabbitCharacter::Attack()
{
	Super::Attack();

	if (AnimInstance)
	{
		AnimInstance->Hit();
	}
}

void APCRRabbitCharacter::Jump()
{
	Super::Jump();

	AnimInstance->Jump();
}

FVector APCRRabbitCharacter::GetRightDiagonal() const
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector RightVector = GetActorRightVector();

	const FVector RightDiagonalVector = (ForwardVector + RightVector).GetSafeNormal();
	const FVector RightDiagonalLocation = GetActorLocation() + RightDiagonalVector * MoveDiagonalDistance;

	return RightDiagonalLocation;
}

FVector APCRRabbitCharacter::GetLeftDiagonal() const
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector RightVector = GetActorRightVector();

	const FVector LeftDiagonalVector = (ForwardVector - RightVector).GetSafeNormal();
	const FVector LeftDiagonalLocation = GetActorLocation() + LeftDiagonalVector * MoveDiagonalDistance;

	return LeftDiagonalLocation;
}
