// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/CRabbitCharacter.h"

#include "Entities/Monsters/Base/CMonsterDataAsset.h"
#include "Entities/Monsters/Base/CMonsterBaseAIController.h"
#include "UI/CUIDataAsset.h"

#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/CParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

ACRabbitCharacter::ACRabbitCharacter()
{
	bCanAttack = true;

	SetActorScale3D(FVector(0.8, 0.8, 0.8));

	if (IsValid(GetParameterDataAsset()))
	{
		MaxHealthPoint = GetParameterDataAsset()->GetRabbitMaxHealthPoint();
		HealthPoint = MaxHealthPoint;
		AttackPower = GetParameterDataAsset()->GetRabbitAttackPower();
		AttackSpeed = GetParameterDataAsset()->GetRabbitAttackSpeed();
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
		GetCharacterMovement()->MaxWalkSpeed = GetParameterDataAsset()->GetRabbitMoveSpeed();
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
		}), AttackSpeed, false);

		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
	}
}

/**
 * TODO: 부모 클래스로 인공지능 정지 기능을 넘길 것
 * 죽으면 인공지능을 정지합니다.
 */
void ACRabbitCharacter::HandleDead()
{
	Super::HandleDead();

	bIsAlive = false;
	
	auto MonsterMonsterAIController = Cast<ACMonsterBaseAIController>(GetController());
	RETURN_IF_INVALID(MonsterMonsterAIController);
	MonsterMonsterAIController->StopMovement();
	RETURN_IF_INVALID(MonsterMonsterAIController->GetBrainComponent());
	MonsterMonsterAIController->GetBrainComponent()->StopLogic("Rabbit is Dead");
}

void ACRabbitCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
