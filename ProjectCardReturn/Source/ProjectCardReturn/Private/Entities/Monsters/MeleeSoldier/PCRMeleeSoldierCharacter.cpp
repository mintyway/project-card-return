// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/PCRMeleeSoldierCharacter.h"

#include "Components/BoxComponent.h"
#include "Entities/Monsters/MeleeSoldier/PCRShieldActor.h"

#include "Components/CapsuleComponent.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogMeleeSoldierCharacter);

APCRMeleeSoldierCharacter::APCRMeleeSoldierCharacter()
{
	bCanAttack = true;
	bOwnShield = false;

	if (IsValid(GetParameterDataAsset()))
	{
		MaxHealthPoint = GetParameterDataAsset()->MeleeSoldierMaxHealthPoint;
		HealthPoint = MaxHealthPoint;
		AttackPower = GetParameterDataAsset()->MeleeSoldierAttackPower;
		AttackRange = GetParameterDataAsset()->MeleeSoldierAttackRange;
		AttackSpeed = GetParameterDataAsset()->MeleeSoldierAttackSpeed;
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

	if (GetCharacterMovement() && GetParameterDataAsset())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = GetParameterDataAsset()->MeleeSoldierMoveSpeed;
	}
}

void APCRMeleeSoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpawnAndAttachShield();
}

void APCRMeleeSoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRMeleeSoldierCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APCRMeleeSoldierCharacter::Attack()
{
	Super::Attack();
}

void APCRMeleeSoldierCharacter::HandleDead()
{
	Super::HandleDead();

	if (bOwnShield)
	{
		Shield->DetachAndDelayedDestroy();
	}
}

/**
 * 실드를 스폰하고 몬스터에게 부착합니다.
 */
void APCRMeleeSoldierCharacter::SpawnAndAttachShield()
{
	RETURN_IF_INVALID(GetWorld());
	Shield = GetWorld()->SpawnActor<APCRShieldActor>();
	RETURN_IF_INVALID(Shield);
	RETURN_IF_INVALID(Shield->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform));
	Shield->SetActorRelativeLocation(FVector(75.0, 0.0, 0.0));
	Shield->OnDetachedShield.AddUObject(this, &APCRMeleeSoldierCharacter::HandleDetachedShield);

	bOwnShield = true;
}

/**
 * 실드가 탈착되고난 후 처리되야할 함수입니다. 실드의 탈착 시점에 바인드 되어있습니다.
 */
void APCRMeleeSoldierCharacter::HandleDetachedShield()
{
	if (!bOwnShield)
	{
		return;
	}

	UE_LOG(PCRLogMeleeSoldierCharacter, Log, TEXT("%s가 %s로부터 분리되었습니다."), *Shield->GetName(), *GetName());
	Shield = nullptr;
	bOwnShield = false;
}
