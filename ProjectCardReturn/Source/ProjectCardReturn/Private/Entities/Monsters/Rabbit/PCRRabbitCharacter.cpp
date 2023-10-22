// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"
#include "UI/PCRUIDataAsset.h"

#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

APCRRabbitCharacter::APCRRabbitCharacter()
{
	if (IsValid(ParameterDataAsset))
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

	// TODO: 더미 모델링 임시 적용: 모델링 작업 완료되면 제거
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cube(TEXT("/Script/Engine.StaticMesh'/Game/Dummy/SM_Cube.SM_Cube'"));
	DummyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyMeshComponent"));
	if (IsValid(DummyMeshComponent) && SM_Cube.Succeeded())
	{
		DummyMeshComponent->SetupAttachment(RootComponent);
		DummyMeshComponent->SetStaticMesh(SM_Cube.Object);
		DummyMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}

	if (IsValid(GetCharacterMovement()) && IsValid(ParameterDataAsset))
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = ParameterDataAsset->RabbitMoveSpeed;
	}
}

void APCRRabbitCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRRabbitCharacter::Attack()
{
	Super::Attack();
	
}

/**
 * TODO: 부모 클래스로 인공지능 정지 기능을 넘길 것
 * 죽으면 인공지능을 정지합니다.
 */
void APCRRabbitCharacter::HandleDead()
{
	Super::HandleDead();
}

void APCRRabbitCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
