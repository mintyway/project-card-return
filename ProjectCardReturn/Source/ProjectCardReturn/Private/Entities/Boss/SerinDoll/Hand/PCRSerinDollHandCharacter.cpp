// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandCharacter.h"

#include "NiagaraComponent.h"
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
	IdleData.ChaseLocationExponentialSpeed = 3.f;
	IdleData.ChaseRotationExponentialSpeed = 2.f;

	RockAttackData = {};
	RockAttackData.bIsChasing = false;
	RockAttackData.ChaseLocationSpeed = 1500.f;
	RockAttackData.ChaseRotationExponentialSpeed = 2.f;
	RockAttackData.ChaseHeight = 500.f;

	PaperAttackData = {};
	PaperAttackData.Lift = nullptr;
	PaperAttackData.Offset = FVector(1250.0, 1000.0, 0.0);
	PaperAttackData.bIsMoving = false;
	PaperAttackData.bIsFar = false;
	PaperAttackData.MoveLocationSpeed = 1500.f;
	PaperAttackData.MoveRotationExponentialSpeed = 2.f;

	ScissorsAttackData = {};
	ScissorsAttackData.bIsChasing = false;
	ScissorsAttackData.ChaseDistance = 500.f;
	ScissorsAttackData.ChaseLocationExponentialSpeed = 1.5f;
	ScissorsAttackData.ChaseRotationExponentialSpeed = 10.f;
	ScissorsAttackData.MaxAttackCount = 3;

	IdleSideOffset = 750.f;
	IdleUpOffset = 300.f;

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

	RockAttackNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RockAttackNiagaraComponent"));
	if (RockAttackNiagaraComponent)
	{
		RockAttackNiagaraComponent->SetupAttachment(GetCapsuleComponent());
		RockAttackNiagaraComponent->SetAsset(SerinDollDataAsset->RockAttackEffect);
	}

	PaperAttackNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PaperAttackNiagaraComponent"));
	if (PaperAttackNiagaraComponent)
	{
		PaperAttackNiagaraComponent->SetupAttachment(GetCapsuleComponent());
		PaperAttackNiagaraComponent->SetAsset(SerinDollDataAsset->PaperAttackEffect);
	}

	ScissorsAttackNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ScissorsAttackNiagaraComponent"));
	if (ScissorsAttackNiagaraComponent)
	{
		ScissorsAttackNiagaraComponent->SetupAttachment(GetCapsuleComponent());
		ScissorsAttackNiagaraComponent->SetRelativeScale3D(FVector(1.5));
		ScissorsAttackNiagaraComponent->SetAsset(SerinDollDataAsset->ScissorsAttackEffect);
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

	RockAttackNiagaraComponent->Deactivate();
	PaperAttackNiagaraComponent->Deactivate();
	ScissorsAttackNiagaraComponent->Deactivate();
	
	CachedSerinDollHandAnimInstance->OnToIdle.BindUObject(this, &APCRSerinDollHandCharacter::HandleToIdle);
	CachedSerinDollHandAnimInstance->OnRockAttackEnded.AddUObject(this, &APCRSerinDollHandCharacter::HandleRockAttackChaseEnded);

	CachedSerinDollHandAnimInstance->OnRockAttackHit.AddUObject(this, &APCRSerinDollHandCharacter::PlayRockAttackEffect);
	CachedSerinDollHandAnimInstance->OnPaperAttackSweepStart.AddUObject(this, &APCRSerinDollHandCharacter::PlayPaperAttackEffect);
	CachedSerinDollHandAnimInstance->OnPaperAttackSweepEnd.AddUObject(this, &APCRSerinDollHandCharacter::StopPaperAttackEffect);
	CachedSerinDollHandAnimInstance->OnScissorsAttackStart.AddUObject(this, &APCRSerinDollHandCharacter::PlayScissorsAttackEffect);
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
			if (RockAttackData.bIsChasing)
			{
				UpdateRockAttackChase(DeltaSeconds);
			}

			break;
		}
		case EState::PaperAttack:
		{
			if (PaperAttackData.bIsMoving)
			{
				UpdatePaperAttackMove(DeltaSeconds);
			}

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

void APCRSerinDollHandCharacter::Init(APCRSerinDollHeadCharacter* NewSerinDollHead, const FVector& InSideVector)
{
	CachedSerinDollHead = NewSerinDollHead;
	SideVector = InSideVector;
	IdleOffsetFromTarget = (SideVector * IdleSideOffset) + (GetActorUpVector() * IdleUpOffset);
	
	const FRotator RelativeInsideRotation = FRotationMatrix::MakeFromX(SideVector).Rotator();
	PaperAttackNiagaraComponent->SetRelativeRotation(RelativeInsideRotation);
}

void APCRSerinDollHandCharacter::Idle(AActor* NewTarget)
{
	IdleData.Target = NewTarget;
	CurrentState = EState::Idle;
}

void APCRSerinDollHandCharacter::RockAttack(AActor* NewTarget)
{
	RockAttackData.Target = NewTarget;
	RockAttackData.bIsChasing = true;
	CurrentState = EState::RockAttack;
	CachedSerinDollHandAnimInstance->PlayRockAttack();
}

void APCRSerinDollHandCharacter::PaperAttack(bool bIsFar)
{
	PaperAttackData.Lift = Cast<AActor>(CachedSerinDollHead->CachedLift);
	PaperAttackData.bIsMoving = true;
	PaperAttackData.bIsFar = bIsFar;
	const FVector Offset = (PaperAttackData.Offset * -CachedSerinDollHead->GetActorForwardVector()) + (PaperAttackData.Offset * SideVector) + (PaperAttackData.Offset * CachedSerinDollHead->GetActorUpVector());
	PaperAttackData.MoveLocation = PaperAttackData.Lift->GetActorLocation() + Offset;
	CurrentState = EState::PaperAttack;
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
	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), TargetOffset, DeltaSeconds, IdleData.ChaseLocationExponentialSpeed);
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), CachedSerinDollHead->GetActorRotation(), DeltaSeconds, IdleData.ChaseRotationExponentialSpeed);
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void APCRSerinDollHandCharacter::UpdateRockAttackChase(float DeltaSeconds)
{
	FVector TargetLocation = RockAttackData.Target->GetActorLocation();
	TargetLocation.Z = CachedSerinDollHead->GetLiftHeight() + RockAttackData.ChaseHeight;
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaSeconds, RockAttackData.ChaseLocationSpeed);
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), CachedSerinDollHead->GetActorRotation(), DeltaSeconds, RockAttackData.ChaseRotationExponentialSpeed);
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void APCRSerinDollHandCharacter::UpdatePaperAttackMove(float DeltaSeconds)
{
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), PaperAttackData.MoveLocation, DeltaSeconds, PaperAttackData.MoveLocationSpeed);
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), CachedSerinDollHead->GetActorRotation(), DeltaSeconds, PaperAttackData.MoveRotationExponentialSpeed);
	SetActorLocationAndRotation(NewLocation, NewRotation);

	const float Distance = FVector::DistSquared(GetActorLocation(), PaperAttackData.MoveLocation);
	if (Distance <= FMath::Square(10.f))
	{
		CachedSerinDollHandAnimInstance->PlayPaperAttack(PaperAttackData.bIsFar);
		PaperAttackData.bIsMoving = false;
	}
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

	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), ScissorsAttackData.Target->GetActorLocation(), DeltaSeconds, ScissorsAttackData.ChaseLocationExponentialSpeed);
	const FVector Direction = (ScissorsAttackData.Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, ScissorsAttackData.ChaseRotationExponentialSpeed);
	SetActorLocationAndRotation(NewLocation, NewRotation);

	const float DistanceSquared = FVector::DistSquared2D(GetActorLocation(), ScissorsAttackData.Target->GetActorLocation());
	if (DistanceSquared <= FMath::Square(ScissorsAttackData.ChaseDistance))
	{
		CachedSerinDollHandAnimInstance->PlayScissorsAttack();
		ScissorsAttackData.bIsChasing = false;
	}
}

void APCRSerinDollHandCharacter::PlayRockAttackEffect()
{
	RockAttackNiagaraComponent->Activate(true);
}

void APCRSerinDollHandCharacter::PlayPaperAttackEffect()
{
	PaperAttackNiagaraComponent->Activate(true);
}

void APCRSerinDollHandCharacter::StopPaperAttackEffect()
{
	PaperAttackNiagaraComponent->Deactivate();
}

void APCRSerinDollHandCharacter::PlayScissorsAttackEffect()
{
	ScissorsAttackNiagaraComponent->Activate(true);
}

void APCRSerinDollHandCharacter::HandleToIdle()
{
	CurrentState = EState::Idle;
}

void APCRSerinDollHandCharacter::HandleRockAttackChaseEnded()
{
	RockAttackData.bIsChasing = false;
}
