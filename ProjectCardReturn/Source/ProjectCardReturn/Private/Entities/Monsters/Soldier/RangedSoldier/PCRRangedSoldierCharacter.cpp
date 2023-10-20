// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/RangedSoldier/PCRRangedSoldierCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Soldier/RangedSoldier/PCRRangedSoldierAnimInstance.h"
#include "Entities/Monsters/Soldier/RangedSoldier/PCRSpearActor.h"
#include "Game/PCRGameModeBase.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

APCRRangedSoldierCharacter::APCRRangedSoldierCharacter()
{
	if (IsValid(GetParameterDataAsset()))
	{
		MaxHealthPoint = GetParameterDataAsset()->RangedSoldierMaxHealthPoint;
		HealthPoint = MaxHealthPoint;
		AttackPower = GetParameterDataAsset()->RangedSoldierAttackPower;
		AttackRange = GetParameterDataAsset()->RangedSoldierAttackRange;
		AttackSpeed = GetParameterDataAsset()->RangedSoldierAttackSpeed;
	}
	
	// TODO: 모델링 작업 완료되면 활성화
	if (GetMesh() && GetMonsterDataAsset())
	{
		GetMesh()->SetSkeletalMesh(GetMonsterDataAsset()->RangedSoldierMesh);
		
		if (UClass* AnimationBlueprint = GetMonsterDataAsset()->RangedSoldierAnimationBlueprint.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(AnimationBlueprint);
		}
	}

	if (GetCharacterMovement() && GetParameterDataAsset())
	{
		GetCharacterMovement()->MaxWalkSpeed = GetParameterDataAsset()->RangedSoldierMoveSpeed;
	}
}

void APCRRangedSoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpawnAndAttachSpear();

	if (const APlayerController* CurrentPlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		CachedPlayerCharacter = CurrentPlayerController->GetCharacter();
	}
}

/**
 * 창을 스폰하고 몬스터에게 부착합니다.
 */
void APCRRangedSoldierCharacter::SpawnAndAttachSpear()
{
	RETURN_IF_INVALID(GetWorld());
	Spear = GetWorld()->SpawnActor<APCRSpearActor>();
	RETURN_IF_INVALID(Spear);
	const FName SocketName = TEXT("Bip001-L-Finger0Socket");
	// RETURN_IF_INVALID(Shield->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	RETURN_IF_INVALID(Spear->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
	// Spear->SetActorRelativeLocation(FVector(75.0, 0.0, 0.0));
	Spear->OnDetachedSpear.AddUObject(this, &APCRRangedSoldierCharacter::HandleDetachedSpear);
	Spear->OnDestroyedSpear.AddUObject(this, &APCRRangedSoldierCharacter::HandleDestroyedSpear);
}

/**
 * 창이 탈착되고난 후 처리되야할 함수입니다. 창의 탈착 시점에 바인드 되어있습니다.
 */
void APCRRangedSoldierCharacter::HandleDetachedSpear()
{
	if (Spear)
	{
		UE_LOG(PCRLogMeleeSoldierCharacter, Log, TEXT("%s가 %s로부터 분리되었습니다."), *Spear->GetName(), *GetName());
		Spear = nullptr;
	}
}

void APCRRangedSoldierCharacter::HandleDestroyedSpear()
{
	Spear = nullptr;

	SpawnAndAttachSpear();
}

void APCRRangedSoldierCharacter::Attack()
{
	Super::Attack();
	
	if (Spear && RangedSoldierAnimInstance)
	{
		RangedSoldierAnimInstance->Throw();
	}
}

void APCRRangedSoldierCharacter::Throw()
{
	if (Spear)
	{
		const float SpearSpeed = GetParameterDataAsset()->SpearSpeed;
		const float Distance = GetDistanceTo(CachedPlayerCharacter);
		const FVector PredictedMove = CachedPlayerCharacter->GetVelocity() * (Distance / SpearSpeed);
		const FVector PredictedLocation = CachedPlayerCharacter->GetActorLocation() + PredictedMove;
		const FVector Direction = (PredictedLocation - GetActorLocation()).GetSafeNormal();
		
		Spear->Throw(this, GetActorLocation(), Direction);
	}
}

void APCRRangedSoldierCharacter::HandleDead()
{
	Super::HandleDead();

	if (Spear)
	{
		Spear->DetachAndDelayedDestroy();
	}
}
