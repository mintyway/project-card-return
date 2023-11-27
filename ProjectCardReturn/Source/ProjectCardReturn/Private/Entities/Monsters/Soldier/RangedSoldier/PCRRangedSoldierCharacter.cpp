// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/RangedSoldier/PCRRangedSoldierCharacter.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Soldier/RangedSoldier/PCRRangedSoldierAnimInstance.h"
#include "Entities/Monsters/Soldier/RangedSoldier/PCRSpearActor.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

APCRRangedSoldierCharacter::APCRRangedSoldierCharacter()
{
	if (ParameterDataAsset)
	{
		MaxHP = ParameterDataAsset->RangedSoldierMaxHP;
		CurrentHP = MaxHP;
		MoveSpeed = ParameterDataAsset->RangedSoldierMoveSpeed;
		AttackPower = ParameterDataAsset->RangedSoldierAttackPower;
		AttackRange = ParameterDataAsset->RangedSoldierAttackRange;
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->InitCapsuleSize(60.f, 87.5f);
	}
	
	// TODO: 모델링 작업 완료되면 활성화
	if (GetMesh() && MonsterDataAsset)
	{
		GetMesh()->SetSkeletalMesh(MonsterDataAsset->RangedSoldierMesh);
		
		if (UClass* AnimationBlueprint = MonsterDataAsset->RangedSoldierAnimationBlueprint.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(AnimationBlueprint);
		}
	}

	if (GetCharacterMovement() && ParameterDataAsset)
	{
		GetCharacterMovement()->MaxWalkSpeed = ParameterDataAsset->RangedSoldierMoveSpeed;
	}

	WarnEffectNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	if (WarnEffectNiagaraComponent && MonsterDataAsset)
	{
		WarnEffectNiagaraComponent->SetAsset(MonsterDataAsset->RangedSoldierWarnEffect);
		WarnEffectNiagaraComponent->SetupAttachment(RootComponent);
		WarnEffectNiagaraComponent->SetAutoActivate(false);
	}
}

void APCRRangedSoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UPCRRangedSoldierAnimInstance>(GetMesh()->GetAnimInstance());
	check(AnimInstance);

	if (const APlayerController* CurrentPlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		CachedPlayerCharacter = CurrentPlayerController->GetCharacter();
	}
}

void APCRRangedSoldierCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	WarnEffectNiagaraComponent->SetWorldRotation(FRotationMatrix::MakeFromX(SpearDirection()).Rotator());
}

void APCRRangedSoldierCharacter::Attack()
{
	Super::Attack();

	WarnEffectNiagaraComponent->Activate(true);

	if (AnimInstance)
	{
		AnimInstance->Throw();
	}
}

void APCRRangedSoldierCharacter::Throw()
{
	check(GetWorld());
	APCRSpearActor* Spear = GetWorld()->SpawnActor<APCRSpearActor>();
	check(Spear);
		
	Spear->Throw(this, GetActorLocation(), SpearDirection());
}

FVector APCRRangedSoldierCharacter::SpearDirection()
{
	const float SpearSpeed = ParameterDataAsset->SpearSpeed;
	const float Distance = GetDistanceTo(CachedPlayerCharacter);
	const FVector PredictedMove = CachedPlayerCharacter->GetVelocity() * (Distance / SpearSpeed);
	const FVector PredictedLocation = CachedPlayerCharacter->GetActorLocation() + PredictedMove * ParameterDataAsset->SpearPredictionRate;
	return (PredictedLocation - GetActorLocation()).GetSafeNormal();
}
