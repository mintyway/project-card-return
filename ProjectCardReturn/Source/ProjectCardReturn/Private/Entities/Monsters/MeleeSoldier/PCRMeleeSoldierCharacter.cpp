// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/PCRMeleeSoldierCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

APCRMeleeSoldierCharacter::APCRMeleeSoldierCharacter()
{
	bCanAttack = true;

	if (IsValid(GetParameterDataAsset()))
	{
		// MaxHealthPoint = GetParameterDataAsset()->RabbitMaxHealthPoint;
		MaxHealthPoint = 1.f;
		HealthPoint = MaxHealthPoint;
		AttackPower = GetParameterDataAsset()->RabbitAttackPower;
		AttackRange = GetParameterDataAsset()->RabbitAttackRange;
		AttackSpeed = GetParameterDataAsset()->RabbitAttackSpeed;
	}

	if (IsValid(GetCapsuleComponent()))
	{
		GetCapsuleComponent()->InitCapsuleSize(50.f, 50.f);
	}

	// TODO: 모델링 작업 완료되면 활성화
	// if (GetMesh() && GetMonsterDataAsset())
	// {
	// 	GetMesh()->SetSkeletalMesh(GetMonsterDataAsset()->GetRabbitMesh());
	// 	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -86.5), FRotator(0.0, -90.0, 0.0));
	// }

	// TODO: 더미 모델링 임시 적용: 모델링 작업 완료되면 제거
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Cube(TEXT("/Script/Engine.StaticMesh'/Game/Dummy/SM_Cube.SM_Cube'"));
	DummyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyMeshComponent"));
	if (IsValid(DummyMeshComponent) && SM_Cube.Succeeded())
	{
		DummyMeshComponent->SetupAttachment(RootComponent);
		DummyMeshComponent->SetStaticMesh(SM_Cube.Object);
		DummyMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}

	if (IsValid(GetCharacterMovement()) && IsValid(GetParameterDataAsset()))
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = GetParameterDataAsset()->RabbitMoveSpeed;
	}
}

void APCRMeleeSoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRMeleeSoldierCharacter::Attack()
{
	Super::Attack();
}

void APCRMeleeSoldierCharacter::HandleDead()
{
	Super::HandleDead();
}

void APCRMeleeSoldierCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
