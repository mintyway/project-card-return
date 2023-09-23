// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/CRabbitCharacter.h"

#include "Entities/Monsters/CMonsterDataAsset.h"
#include "UI/CUIDataAsset.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACRabbitCharacter::ACRabbitCharacter()
{
	bCanAttack = true;

	// TODO: 파라미터화 필요
	MaxHealthPoint = 100.f;
	HealthPoint = MaxHealthPoint;
	// TODO: 파라미터화 필요
	AttackPower = 3.f;

	SetActorScale3D(FVector(0.8, 0.8, 0.8));

	// TODO: AI컨트롤러 추가
	// AIControllerClass = ;
	// AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->InitCapsuleSize(45.f, 86.5f);
	}
	
	if (GetMesh() && GetMonsterDataAsset())
	{
		GetMesh()->SetSkeletalMesh(GetMonsterDataAsset()->GetRabbitMesh());
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -86.5), FRotator(0.0, -90.0, 0.0));
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// TODO: 파라미터화 필요
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

void ACRabbitCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACRabbitCharacter::Attack()
{
	Super::Attack();

	if (bCanAttack)
	{
		bCanAttack = false;
		FTimerHandle UnUsedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnUsedHandle, FTimerDelegate::CreateLambda([this]() -> void
		{
			SIMPLE_LOG;
			bCanAttack = true;
		}), 1.f/*TODO: 파라미터화 필요*/, false);
		
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
	}
}

/**
 * TODO: 부모 클래스로 인공지능 정지 기능을 넘길 것
 * 죽으면 인공지능을 정지합니다.
 */
void ACRabbitCharacter::Dead()
{
	Super::Dead();

	// auto RabbitAIController = Cast<AAIController>(GetController());
	// RETURN_IF_INVALID(RabbitAIController);
	// RabbitAIController->StopMovement();
	// RETURN_IF_INVALID(RabbitAIController->GetBrainComponent());
	// RabbitAIController->GetBrainComponent()->StopLogic("Rabbit is Dead");
}

void ACRabbitCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
