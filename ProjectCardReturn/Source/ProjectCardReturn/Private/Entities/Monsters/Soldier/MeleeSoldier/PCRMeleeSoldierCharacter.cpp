// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierAnimInstance.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/PCRShieldActor.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

APCRMeleeSoldierCharacter::APCRMeleeSoldierCharacter()
{
	if (IsValid(GetParameterDataAsset()))
	{
		MaxHealthPoint = GetParameterDataAsset()->MeleeSoldierMaxHealthPoint;
		HealthPoint = MaxHealthPoint;
		AttackPower = GetParameterDataAsset()->MeleeSoldierAttackPower;
		AttackRange = GetParameterDataAsset()->MeleeSoldierAttackRange;
		AttackSpeed = GetParameterDataAsset()->MeleeSoldierAttackSpeed;
	}
	
	// TODO: 모델링 작업 완료되면 활성화
	if (GetMesh() && GetMonsterDataAsset())
	{
		GetMesh()->SetSkeletalMesh(GetMonsterDataAsset()->MeleeSoldierMesh);
		
		if (UClass* AnimationBlueprint = GetMonsterDataAsset()->MeleeSoldierAnimationBlueprint.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(AnimationBlueprint);
		}
	}

	if (GetCharacterMovement() && GetParameterDataAsset())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetParameterDataAsset()->MeleeSoldierMoveSpeed;
	}
}

void APCRMeleeSoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeleeSoldierAnimInstance = Cast<UPCRMeleeSoldierAnimInstance>(GetMesh()->GetAnimInstance());
	if (!MeleeSoldierAnimInstance)
	{
		NULL_POINTER_EXCEPTION(MeleeSoldierAnimInstance);
	}

	SpawnAndAttachShield();
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
}

/**
 * 실드가 탈착되고난 후 처리되야할 함수입니다. 실드의 탈착 시점에 바인드 되어있습니다.
 */
void APCRMeleeSoldierCharacter::HandleDetachedShield()
{
	if (Shield)
	{
		UE_LOG(PCRLogMeleeSoldierCharacter, Log, TEXT("%s가 %s로부터 분리되었습니다."), *Shield->GetName(), *GetName());
		Shield = nullptr;
	}
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

	if (Shield)
	{
		Shield->DetachAndDelayedDestroy();
	}
}
