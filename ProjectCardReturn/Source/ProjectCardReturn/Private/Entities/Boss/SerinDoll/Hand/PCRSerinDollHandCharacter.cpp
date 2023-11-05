// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandAnimInstance.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogSerinHandCharacter);

APCRSerinDollHandCharacter::APCRSerinDollHandCharacter()
	: CurrentState(EState::Idle)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// TODO: 파라미터화 필요
	DefaultSpeed = 1000.f;
	IdleData = {};
	IdleData.ChaseExponentialSpeed = 2.f;
	ScissorsAttackData = {};
	ScissorsAttackData.ChaseDistance = 500.f;
	ScissorsAttackData.ChaseExponentialSpeed = 1.f;
	ScissorsAttackData.ChaseRotationExponentialSpeed = 10.f;
	ScissorsAttackData.MaxAttackCount = 3;

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	}

	if (GetMesh() && SerinDollDataAsset)
	{
		GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetRelativeScale3D(FVector(0.25, 0.25, 0.25));

		GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel9);
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
		GetMesh()->SetGenerateOverlapEvents(true);

		GetMesh()->SetSkeletalMesh(SerinDollDataAsset->HandMesh);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		if (UClass* NewAnimInstanceClass = SerinDollDataAsset->AnimInstanceClass.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(NewAnimInstanceClass);
		}
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
		GetCharacterMovement()->bCheatFlying = true;
	}
}

void APCRSerinDollHandCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

#if WITH_EDITOR
	if (!GetWorld()->IsPlayInEditor())
	{
		return;
	}
#endif

	CachedSerinDollHandAnimInstance = Cast<UPCRSerinDollHandAnimInstance>(GetMesh()->GetAnimInstance());
	check(CachedSerinDollHandAnimInstance);

	CachedSerinDollHandAnimInstance->OnToIdle.BindUObject(this, &APCRSerinDollHandCharacter::HandleToIdle);
}

void APCRSerinDollHandCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSerinDollHandCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (CurrentState)
	{
		case EState::Idle:
		{
			UpdateIdle(DeltaSeconds);
			break;
		}
		case EState::Move:
		{
			break;
		}
		case EState::Chase:
		{
			break;
		}
		case EState::RockAttack:
		{
			break;
		}
		case EState::PaperAttack:
		{
			break;
		}
		case EState::ScissorsAttack:
		{
			if (ScissorsAttackData.bIsChasing)
			{
				UpdateScissorsAttackChase(DeltaSeconds);
			}

			break;
		}
	}
}

float APCRSerinDollHandCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Damage *= 1.5;
	CachedSerinDollHead->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}

void APCRSerinDollHandCharacter::Init(APCRSerinDollHeadCharacter* NewSerinDollHead, const FVector& InIdleOffsetFromTarget)
{
	CachedSerinDollHead = NewSerinDollHead;
	IdleOffsetFromTarget = InIdleOffsetFromTarget;
}

void APCRSerinDollHandCharacter::Idle(AActor* NewTarget)
{
	IdleData.Target = NewTarget;
	CurrentState = EState::Idle;
}

void APCRSerinDollHandCharacter::ScissorsAttack(AActor* NewTarget)
{
	ScissorsAttackData.Target = NewTarget;
	ScissorsAttackData.bIsChasing = true;
	ScissorsAttackData.CurrentAttackCount = 0;
	CurrentState = EState::ScissorsAttack;
}

void APCRSerinDollHandCharacter::UpdateIdle(float DeltaSeconds)
{
	const FVector TargetOffset = IdleData.Target->GetActorLocation() + IdleOffsetFromTarget;
	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetOffset, DeltaSeconds, IdleData.ChaseExponentialSpeed);
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), CachedSerinDollHead->GetActorRotation(), DeltaSeconds, IdleData.ChaseExponentialSpeed);
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void APCRSerinDollHandCharacter::UpdateScissorsAttackChase(float DeltaSeconds)
{
	// 높이 맞추기
	if ((GetActorLocation().Z - ScissorsAttackData.Target->GetActorLocation().Z) >= 5.f)
	{
		const float NewHeight = FMath::FInterpConstantTo(GetActorLocation().Z, ScissorsAttackData.Target->GetActorLocation().Z, DeltaSeconds, DefaultSpeed);
		FVector NewLocation = GetActorLocation();
		NewLocation.Z = NewHeight;
		const FVector Direction = (ScissorsAttackData.Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, ScissorsAttackData.ChaseRotationExponentialSpeed);
		SetActorLocationAndRotation(NewLocation, NewRotation);

		return;
	}

	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), ScissorsAttackData.Target->GetActorLocation(), DeltaSeconds, ScissorsAttackData.ChaseExponentialSpeed);
	const FVector Direction = (ScissorsAttackData.Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, ScissorsAttackData.ChaseRotationExponentialSpeed);
	SetActorLocationAndRotation(NewLocation, NewRotation);

	const float DistanceSquared = FVector::DistSquared2D(GetActorLocation(), ScissorsAttackData.Target->GetActorLocation());
	if (DistanceSquared <= FMath::Square(ScissorsAttackData.ChaseDistance))
	{
		CachedSerinDollHandAnimInstance->PlayScissorsAttack(this);
		ScissorsAttackData.bIsChasing = false;
	}
}

void APCRSerinDollHandCharacter::HandleToIdle()
{
	CurrentState = EState::Idle;
}
