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

	RangedSoldierAnimInstance = Cast<UPCRRangedSoldierAnimInstance>(GetMesh()->GetAnimInstance());
	check(RangedSoldierAnimInstance);

	if (const APlayerController* CurrentPlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		CachedPlayerCharacter = CurrentPlayerController->GetCharacter();
	}
}

void APCRRangedSoldierCharacter::Attack()
{
	Super::Attack();
	
	if (RangedSoldierAnimInstance)
	{
		RangedSoldierAnimInstance->Throw();
	}
}

void APCRRangedSoldierCharacter::Throw()
{
	check(GetWorld());
	APCRSpearActor* Spear = GetWorld()->SpawnActor<APCRSpearActor>();
	check(Spear);
	
	const float SpearSpeed = GetParameterDataAsset()->SpearSpeed;
	const float Distance = GetDistanceTo(CachedPlayerCharacter);
	const FVector PredictedMove = CachedPlayerCharacter->GetVelocity() * (Distance / SpearSpeed);
	const FVector PredictedLocation = CachedPlayerCharacter->GetActorLocation() + PredictedMove;
	const FVector Direction = (PredictedLocation - GetActorLocation()).GetSafeNormal();
		
	Spear->Throw(this, GetActorLocation(), Direction);
}
