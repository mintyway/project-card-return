// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/PCRMeleeSoldierCharacter.h"

#include "Components/BoxComponent.h"
#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/MeleeSoldier/PCRShieldActor.h"
#include "Entities/Monsters/MeleeSoldier/PCRSpearActor.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Monsters/MeleeSoldier/PCRMeleeSoldierAnimInstance.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogMeleeSoldierCharacter);

APCRMeleeSoldierCharacter::APCRMeleeSoldierCharacter()
{
	bCanAttack = true;
	bHasShield = false;
	//
	bHasSpear = false;

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
	if (GetMesh() && GetMonsterDataAsset())
	{
		GetMesh()->SetupAttachment(GetCapsuleComponent());
		GetMesh()->SetSkeletalMesh(GetMonsterDataAsset()->MeleeSoldierMesh);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -86.5), FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		if (UClass* AnimationBlueprint = GetMonsterDataAsset()->MeleeSoldierAnimationBlueprint.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(AnimationBlueprint);
		}
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

	MeleeSoldierAnimInstance = Cast<UPCRMeleeSoldierAnimInstance>(GetMesh()->GetAnimInstance());
	if (!MeleeSoldierAnimInstance)
	{
		NULL_POINTER_EXCEPTION(MeleeSoldierAnimInstance);
	}
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

	if (MeleeSoldierAnimInstance)
	{
		MeleeSoldierAnimInstance->Attack();
	}
}

void APCRMeleeSoldierCharacter::HandleDead()
{
	Super::HandleDead();

	if (bHasShield)
	{
		Shield->DetachAndDelayedDestroy();
	}

	//
	if (bHasSpear)
	{
		Spear->DetachAndDelayedDestroy();	
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
	const FName SocketName = TEXT("Bip001-L-Finger0Socket");
	// RETURN_IF_INVALID(Shield->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	RETURN_IF_INVALID(Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	// Shield->SetActorRelativeLocation(FVector(75.0, 0.0, 0.0));
	Shield->OnDetachedShield.AddUObject(this, &APCRMeleeSoldierCharacter::HandleDetachedShield);

	bHasShield = true;
}

//
void APCRMeleeSoldierCharacter::SpawnAndAttachSpear()
{
	RETURN_IF_INVALID(GetWorld());
	Spear = GetWorld()->SpawnActor<APCRSpearActor>();
	RETURN_IF_INVALID(Spear);
	const FName SocketName = TEXT("Bip001-L-Finger0Socket");
	// RETURN_IF_INVALID(Shield->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	RETURN_IF_INVALID(Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	// Spear->SetActorRelativeLocation(FVector(75.0, 0.0, 0.0));

	bHasSpear = true;
}

/**
 * 실드가 탈착되고난 후 처리되야할 함수입니다. 실드의 탈착 시점에 바인드 되어있습니다.
 */
void APCRMeleeSoldierCharacter::HandleDetachedShield()
{
	if (!bHasShield)
	{
		return;
	}

	UE_LOG(PCRLogMeleeSoldierCharacter, Log, TEXT("%s가 %s로부터 분리되었습니다."), *Shield->GetName(), *GetName());
	Shield = nullptr;
	bHasShield = false;
}
