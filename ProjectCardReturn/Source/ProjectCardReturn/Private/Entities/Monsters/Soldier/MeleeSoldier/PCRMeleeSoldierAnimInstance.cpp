// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierAnimInstance.h"

#include "Engine/DamageEvents.h"
#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierCharacter.h"

DEFINE_LOG_CATEGORY(PCRLogMeleeSoldierAnimInstance);

UPCRMeleeSoldierAnimInstance::UPCRMeleeSoldierAnimInstance(): bHasShield(false), bCanAttack(true) {}

void UPCRMeleeSoldierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedMeleeSoldier = Cast<APCRMeleeSoldierCharacter>(CachedMonsterBaseCharacter);
	if (!CachedMeleeSoldier)
	{
		NULL_POINTER_EXCEPTION(CachedMeleeSoldier);
	}

	OnMontageEnded.AddDynamic(this, &UPCRMeleeSoldierAnimInstance::AttackMontageEnded);
}

void UPCRMeleeSoldierAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CachedMeleeSoldier)
	{
		bHasShield = CachedMeleeSoldier->GetHasShield();
	}
}

void UPCRMeleeSoldierAnimInstance::Attack()
{
	if (bCanAttack)
	{
		UE_LOG(PCRLogMeleeSoldierAnimInstance, Warning, TEXT("Attack!"));
	
		if (bHasShield)
		{
			ShieldAttack();
		}
		else
		{
			SpearAttack();
		}

		bCanAttack = false;
	}
}

void UPCRMeleeSoldierAnimInstance::AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bCanAttack = true;
}

void UPCRMeleeSoldierAnimInstance::AnimNotify_Hit()
{
	check(CachedMeleeSoldier);

	// TODO: 파라미터화 필요 리스트
	float AttackRange = CachedMeleeSoldier->GetAttackRange();
	float AttackRadius = 50.f;

	FHitResult HitResult;
	const FVector Start = CachedMeleeSoldier->GetActorLocation();
	const FVector End = Start + CachedMeleeSoldier->GetActorForwardVector() * AttackRange;
	const FQuat Rot = FQuat::Identity;
	FCollisionShape Shape = FCollisionShape::MakeSphere(AttackRadius);
	const bool bSweepResult = GetWorld()->SweepSingleByChannel(HitResult, Start, End, Rot, ECC_GameTraceChannel7, Shape);
	if (bSweepResult)
	{
		if (AActor* TargetActor = HitResult.GetActor())
		{
			float AttackDamage = CachedMeleeSoldier->GetAttackPower();
			FDamageEvent DamageEvent;
			TargetActor->TakeDamage(AttackDamage, DamageEvent, CachedMeleeSoldier->GetController(), CachedMeleeSoldier);
		}
	}

	const FVector TraceVector = CachedMeleeSoldier->GetActorForwardVector() * AttackRange;
	const FVector Center = Start + TraceVector * 0.5f;
	const float HalfHeight = AttackRange * 0.5f + AttackRadius;
	const FQuat CapsuleRotate = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();
	const FColor DrawColor = bSweepResult ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRotate, DrawColor, false, 1.f);
}

void UPCRMeleeSoldierAnimInstance::ShieldAttack()
{
	Montage_Play(MonsterDataAsset->MeleeSoldierShieldAttackAnimationMontage);
}

void UPCRMeleeSoldierAnimInstance::SpearAttack()
{
	Montage_Play(MonsterDataAsset->MeleeSoldierSpearAttackAnimationMontage);
}
