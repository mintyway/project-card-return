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

	DetachShieldAndDestroy();
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
	Shield->OnDetachedCard.AddUObject(this, &APCRMeleeSoldierCharacter::DetachShieldAndDestroy);

	bOwnShield = true;
}

/**
 * 실드를 탈착하고 물리 시뮬레이션을 활성화합니다. 이후 파괴합니다.
 */
void APCRMeleeSoldierCharacter::DetachShieldAndDestroy()
{
	if (!bOwnShield)
	{
		return;
	}

	FTimerHandle DestroyTimer;
	GetWorldTimerManager().SetTimer(DestroyTimer, FTimerDelegate::CreateLambda([this]() -> void
	{
		Shield->Destroy();
		Shield = nullptr;
	}), 1.f, false);

	Shield->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	UBoxComponent* ShieldBoxComponent = Shield->FindComponentByClass<UBoxComponent>();
	RETURN_IF_INVALID(ShieldBoxComponent);
	ShieldBoxComponent->SetSimulatePhysics(true);

	UE_LOG(PCRLogMeleeSoldierCharacter, Log, TEXT("%s가 %s로부터 분리되었습니다."), *Shield->GetName(), *GetName());

	bOwnShield = false;
}
