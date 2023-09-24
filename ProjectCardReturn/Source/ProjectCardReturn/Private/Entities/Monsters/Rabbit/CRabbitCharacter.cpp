// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/CRabbitCharacter.h"

#include "Entities/Monsters/Base/CMonsterDataAsset.h"
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
	if (DummyMeshComponent && SM_Cube.Succeeded())
	{
		DummyMeshComponent->SetupAttachment(RootComponent);
		DummyMeshComponent->SetStaticMesh(SM_Cube.Object);
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
void ACRabbitCharacter::HandleDead()
{
	Super::HandleDead();

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
