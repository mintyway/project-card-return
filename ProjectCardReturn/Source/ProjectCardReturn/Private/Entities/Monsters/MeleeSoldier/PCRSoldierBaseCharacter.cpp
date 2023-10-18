// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/PCRSoldierBaseCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Monsters/MeleeSoldier/PCRMeleeSoldierAnimInstance.h"
#include "Entities/Monsters/MeleeSoldier/PCRShieldActor.h"
#include "Entities/Monsters/MeleeSoldier/PCRSpearActor.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogMeleeSoldierCharacter);

APCRSoldierBaseCharacter::APCRSoldierBaseCharacter()
{
	bCanAttack = true;
	
	if (IsValid(GetParameterDataAsset()))
	{
		MaxHealthPoint = GetParameterDataAsset()->MeleeSoldierMaxHealthPoint;
		HealthPoint = MaxHealthPoint;
		AttackPower = GetParameterDataAsset()->MeleeSoldierAttackPower;
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

void APCRSoldierBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeleeSoldierAnimInstance = Cast<UPCRMeleeSoldierAnimInstance>(GetMesh()->GetAnimInstance());
	if (!MeleeSoldierAnimInstance)
	{
		NULL_POINTER_EXCEPTION(MeleeSoldierAnimInstance);
	}
}

void APCRSoldierBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSoldierBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

/**
 * 실드를 스폰하고 몬스터에게 부착합니다.
 */
void APCRSoldierBaseCharacter::SpawnAndAttachShield()
{
	RETURN_IF_INVALID(GetWorld());
	Shield = GetWorld()->SpawnActor<APCRShieldActor>();
	RETURN_IF_INVALID(Shield);
	const FName SocketName = TEXT("Bip001-L-Finger0Socket");
	// RETURN_IF_INVALID(Shield->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	RETURN_IF_INVALID(Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	// Shield->SetActorRelativeLocation(FVector(75.0, 0.0, 0.0));
	Shield->OnDetachedShield.AddUObject(this, &APCRSoldierBaseCharacter::HandleDetachedShield);

	bHasShield = true;
}

/**
 * 실드가 탈착되고난 후 처리되야할 함수입니다. 실드의 탈착 시점에 바인드 되어있습니다.
 */
void APCRSoldierBaseCharacter::HandleDetachedShield()
{
	if (!bHasShield)
	{
		return;
	}

	UE_LOG(PCRLogMeleeSoldierCharacter, Log, TEXT("%s가 %s로부터 분리되었습니다."), *Shield->GetName(), *GetName());
	Shield = nullptr;
	bHasShield = false;
}

/**
 * 창을 스폰하고 몬스터에게 부착합니다.
 */
void APCRSoldierBaseCharacter::SpawnAndAttachSpear()
{
	RETURN_IF_INVALID(GetWorld());
	Spear = GetWorld()->SpawnActor<APCRSpearActor>();
	RETURN_IF_INVALID(Spear);
	const FName SocketName = TEXT("Bip001-L-Finger0Socket");
	// RETURN_IF_INVALID(Shield->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	RETURN_IF_INVALID(Spear->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	// Spear->SetActorRelativeLocation(FVector(75.0, 0.0, 0.0));
	Spear->OnDetachedSpear.AddUObject(this, &APCRSoldierBaseCharacter::HandleDetachedSpear);

	bHasSpear = true;
}

/**
 * 창이 탈착되고난 후 처리되야할 함수입니다. 창의 탈착 시점에 바인드 되어있습니다.
 */
void APCRSoldierBaseCharacter::HandleDetachedSpear()
{
	if (!bHasSpear)
	{
		return;
	}

	UE_LOG(PCRLogMeleeSoldierCharacter, Log, TEXT("%s가 %s로부터 분리되었습니다."), *Spear->GetName(), *GetName());
	Spear = nullptr;
	bHasSpear = false;
}

void APCRSoldierBaseCharacter::Attack()
{
	Super::Attack();

	if (MeleeSoldierAnimInstance)
	{
		MeleeSoldierAnimInstance->Attack();
	}
}

void APCRSoldierBaseCharacter::HandleDead()
{
	Super::HandleDead();

	if (bHasShield)
	{
		Shield->DetachAndDelayedDestroy();
	}

	if (bHasSpear)
	{
		Spear->DetachAndDelayedDestroy();
	}
}
