// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierAnimInstance.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/PCRShieldActor.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

APCRMeleeSoldierCharacter::APCRMeleeSoldierCharacter()
{
	if (ParameterDataAsset)
	{
		MaxHP = ParameterDataAsset->MeleeSoldierMaxHP;
		CurrentHP = MaxHP;
		AttackPower = ParameterDataAsset->MeleeSoldierAttackPower;
		AttackRange = ParameterDataAsset->MeleeSoldierAttackRange;
		AttackRate = ParameterDataAsset->MeleeSoldierAttackRate;
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->InitCapsuleSize(60.f, 87.5f);
	}
	
	// TODO: 모델링 작업 완료되면 활성화
	if (GetMesh() && MonsterDataAsset)
	{
		GetMesh()->SetSkeletalMesh(MonsterDataAsset->MeleeSoldierMesh);
		
		if (UClass* AnimationBlueprint = MonsterDataAsset->MeleeSoldierAnimationBlueprint.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(AnimationBlueprint);
		}
	}

	if (GetCharacterMovement() && ParameterDataAsset)
	{
		GetCharacterMovement()->MaxWalkSpeed = ParameterDataAsset->MeleeSoldierMoveSpeed;
	}
}

void APCRMeleeSoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UPCRMeleeSoldierAnimInstance>(GetMesh()->GetAnimInstance());
	check(AnimInstance);

	SpawnAndAttachShield();
}

void APCRMeleeSoldierCharacter::Attack()
{
	Super::Attack();

	if (AnimInstance)
	{
		AnimInstance->Hit();
	}
}

/**
 * 실드를 스폰하고 몬스터에게 부착합니다.
 */
void APCRMeleeSoldierCharacter::SpawnAndAttachShield()
{
	check(GetWorld());
	Shield = GetWorld()->SpawnActor<APCRShieldActor>();
	check(Shield);
	const FName SocketName = TEXT("Bip001-L-Finger0NubSocket");
	verify(Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName));
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

void APCRMeleeSoldierCharacter::Hit()
{
	// TODO: 파라미터화 필요 리스트
	float AttackRadius = 50.f;

	FHitResult HitResult;
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * AttackRange;
	const FQuat Rot = FQuat::Identity;
	FCollisionShape Shape = FCollisionShape::MakeSphere(AttackRadius);
	const bool bSweepResult = GetWorld()->SweepSingleByChannel(HitResult, Start, End, Rot, ECC_GameTraceChannel7, Shape);
	
	if (bSweepResult)
	{
		if (AActor* TargetActor = HitResult.GetActor())
		{
			float AttackDamage = GetAttackPower();
			FDamageEvent DamageEvent;
			TargetActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
	}

	const FVector TraceVector = GetActorForwardVector() * AttackRange;
	const FVector Center = Start + TraceVector * 0.5f;
	const float HalfHeight = AttackRange * 0.5f + AttackRadius;
	const FQuat CapsuleRotate = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();
	const FColor DrawColor = bSweepResult ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRotate, DrawColor, false, 1.f);
}

void APCRMeleeSoldierCharacter::HandleDead()
{
	Super::HandleDead();

	if (Shield)
	{
		Shield->DetachAndDelayedDestroy();
	}
}
