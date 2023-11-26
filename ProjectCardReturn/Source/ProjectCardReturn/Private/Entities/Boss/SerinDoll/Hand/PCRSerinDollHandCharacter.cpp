// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandCharacter.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandAnimInstance.h"
#include "Entities/Boss/SerinDoll/Projectile/PCRSerinDollPattern1Projectile.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
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
	RockAttackData.Damage = 20.f;

	PaperAttackData = {};
	PaperAttackData.Lift = nullptr;
	PaperAttackData.Offset = FVector(1250.0, 1000.0, 0.0);
	PaperAttackData.bIsMoving = false;
	PaperAttackData.bIsFarAttack = false;
	PaperAttackData.MoveLocationSpeed = 1500.f;
	PaperAttackData.MoveRotationExponentialSpeed = 2.f;
	PaperAttackData.Damage = 20.f;

	ScissorsAttackData = {};
	ScissorsAttackData.bIsChasing = false;
	ScissorsAttackData.ChaseDistance = 500.f;
	ScissorsAttackData.ChaseLocationExponentialSpeed = 1.5f;
	ScissorsAttackData.ChaseRotationExponentialSpeed = 10.f;
	ScissorsAttackData.MaxAttackCount = 3;
	ScissorsAttackData.Damage = 10.f;

	Pattern1Data = {};
	Pattern1Data.Offset = FVector(1500.0, 1000.0, 0.0);
	Pattern1Data.Pattern1TargetLocationIndex = 0;

	Pattern2Data = {};
	Pattern2Data.Offset = FVector(1200.0, 750.0, -20.0);
	Pattern2Data.AttachedCardCount = 0;

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
		InitMeshCollision();

		GetMesh()->SetSkeletalMesh(SerinDollDataAsset->HandMesh);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		if (UClass* NewAnimInstanceClass = SerinDollDataAsset->HandAnimInstanceClass.LoadSynchronous())
		{
			GetMesh()->SetAnimInstanceClass(NewAnimInstanceClass);
		}
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
		GetCharacterMovement()->bCheatFlying = true;
	}

	PaperAttackSweepPlane = CreateDefaultSubobject<UBoxComponent>(TEXT("PaperAttackSweepPlane"));
	if (PaperAttackSweepPlane)
	{
		PaperAttackSweepPlane->SetupAttachment(GetCapsuleComponent());
		PaperAttackSweepPlane->SetRelativeLocation(FVector(80.0, 0.0, 0.0));
		PaperAttackSweepPlane->InitBoxExtent(FVector(500.0, 100.0, 500.0));
		PaperAttackSweepPlane->SetCollisionObjectType(ECC_GameTraceChannel9);
		PaperAttackSweepPlane->SetCollisionResponseToAllChannels(ECR_Ignore);
		PaperAttackSweepPlane->SetGenerateOverlapEvents(true);
	}

	ScissorsAttackHitPlane = CreateDefaultSubobject<UBoxComponent>(TEXT("ScissorsAttackHitPlane"));
	if (ScissorsAttackHitPlane)
	{
		ScissorsAttackHitPlane->SetupAttachment(GetCapsuleComponent());
		ScissorsAttackHitPlane->SetRelativeLocation(FVector(300.0, 0.0, 0.0));
		ScissorsAttackHitPlane->InitBoxExtent(FVector(300.0, 200.0, 200.0));
		ScissorsAttackHitPlane->SetCollisionObjectType(ECC_GameTraceChannel9);
		ScissorsAttackHitPlane->SetCollisionResponseToAllChannels(ECR_Ignore);
		ScissorsAttackHitPlane->SetGenerateOverlapEvents(true);
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
	CachedSerinDollHandAnimInstance->OnRockAttackHit.AddUObject(this, &APCRSerinDollHandCharacter::HandleRockAttackHit);

	PaperAttackSweepPlane->OnComponentBeginOverlap.AddDynamic(this, &APCRSerinDollHandCharacter::HandlePaperAttackHit);
	CachedSerinDollHandAnimInstance->OnPaperAttackSweepStart.AddUObject(this, &APCRSerinDollHandCharacter::HandlePaperAttackSweepStart);
	CachedSerinDollHandAnimInstance->OnPaperAttackSweepEnd.AddUObject(this, &APCRSerinDollHandCharacter::HandlePaperAttackSweepEnd);

	ScissorsAttackHitPlane->OnComponentBeginOverlap.AddDynamic(this, &APCRSerinDollHandCharacter::HandleScissorsAttackOverlapped);
	CachedSerinDollHandAnimInstance->OnScissorsAttackEffectStart.AddUObject(this, &APCRSerinDollHandCharacter::HandleScissorsAttackEffectStart);
	CachedSerinDollHandAnimInstance->OnScissorsAttackHitStart.AddUObject(this, &APCRSerinDollHandCharacter::HandleScissorsAttackHitStart);
	CachedSerinDollHandAnimInstance->OnScissorsAttackHitEnd.AddUObject(this, &APCRSerinDollHandCharacter::HandleScissorsAttackHitEnd);

	CachedSerinDollHandAnimInstance->OnPattern1Shoot.AddUObject(this, &APCRSerinDollHandCharacter::Pattern1Shoot);
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
		case EState::Pattern1:
		{
			break;
		}
		case EState::Pattern2:
		{
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

void APCRSerinDollHandCharacter::PaperAttack(bool bIsFarAttack)
{
	PaperAttackData.Lift = Cast<AActor>(CachedSerinDollHead->CachedLift);
	PaperAttackData.bIsMoving = true;
	PaperAttackData.bIsFarAttack = bIsFarAttack;
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

void APCRSerinDollHandCharacter::ResetAllAttack()
{
	HandlePaperAttackSweepEnd();
	HandleScissorsAttackHitEnd();
}

void APCRSerinDollHandCharacter::Pattern1(const TArray<FVector>& Pattern1TargetLocations)
{
	const APCRLiftActor* Lift = Cast<APCRLiftActor>(CachedSerinDollHead->CachedLift);
	const FVector Offset = (Pattern1Data.Offset * -CachedSerinDollHead->GetActorForwardVector()) + (Pattern1Data.Offset * SideVector) + (Pattern1Data.Offset * CachedSerinDollHead->GetActorUpVector());
	const FVector NewLocation = Lift->GetActorLocation() + Offset;
	const FRotator NewRotation = CachedSerinDollHead->GetActorRotation();
	SetActorLocationAndRotation(NewLocation, NewRotation);
	Pattern1Data.Pattern1TargetLocations = Pattern1TargetLocations;

	const bool IsLeftHand = SideVector == CachedSerinDollHead->GetActorRightVector() ? false : true;
	CachedSerinDollHandAnimInstance->PlayPattern1(IsLeftHand);
	Pattern1Data.Pattern1TargetLocationIndex = IsLeftHand ? 3 : 0;

	CurrentState = EState::Pattern1;
}

void APCRSerinDollHandCharacter::Pattern2()
{
	const APCRLiftActor* Lift = Cast<APCRLiftActor>(CachedSerinDollHead->CachedLift);
	const FVector Offset = (Pattern2Data.Offset * -CachedSerinDollHead->GetActorForwardVector()) + (Pattern2Data.Offset * SideVector) + (Pattern2Data.Offset * CachedSerinDollHead->GetActorUpVector());
	const FVector NewLocation = Lift->GetActorLocation() + Offset;
	const FRotator NewRotation = CachedSerinDollHead->GetActorRotation();
	SetActorLocationAndRotation(NewLocation, NewRotation);

	HandlePatter2Start();
	CachedSerinDollHandAnimInstance->PlayPattern2();

	CurrentState = EState::Pattern2;
}

void APCRSerinDollHandCharacter::Pattern2ResetAttachedCardCount()
{
	Pattern2Data.AttachedCardCount = 0;
}

void APCRSerinDollHandCharacter::Pattern2End()
{
	CachedSerinDollHandAnimInstance->EndPatter2();
	InitMeshCollision();
}

void APCRSerinDollHandCharacter::BindOnReturnCardBegin(APCREricaCardProjectile* AttachedCard)
{
	++Pattern2Data.AttachedCardCount;
	CachedSerinDollHandAnimInstance->HitPattern2();
	UE_LOG(PCRLogSerinHandCharacter, Warning, TEXT("박힌 카드 개수: %d"), Pattern2Data.AttachedCardCount);
	AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRSerinDollHandCharacter::HandlePattern2CardPull);
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
		CachedSerinDollHandAnimInstance->PlayPaperAttack(PaperAttackData.bIsFarAttack);
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

void APCRSerinDollHandCharacter::InitMeshCollision()
{
	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel9);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
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

void APCRSerinDollHandCharacter::HandleScissorsAttackEffectStart()
{
	ScissorsAttackNiagaraComponent->Activate(true);
}

void APCRSerinDollHandCharacter::HandleRockAttackHit()
{
	TArray<FOverlapResult> OverlappedPlayers;
	const FVector CenterLocation = GetActorLocation();
	const FQuat Rotation = FQuat::Identity;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	const float SphereRadius = 500.f;
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereRadius);
	const bool bSucceedOverlap = GetWorld()->OverlapMultiByObjectType(OverlappedPlayers, CenterLocation, Rotation, ObjectQueryParams, CollisionShape);

	float ActualDamage = 0.f;
	if (bSucceedOverlap)
	{
		for (const auto& OverlappedPlayer : OverlappedPlayers)
		{
			const auto Actor = OverlappedPlayer.GetActor();
			check(Actor);
			const FDamageEvent DamageEvent;
			ActualDamage = Actor->TakeDamage(RockAttackData.Damage, DamageEvent, CachedSerinDollHead->GetController(), this);

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
									   SerinDollDataAsset->RockAttackHitEffect,
									   Actor->GetActorLocation());
		}
	}

	const FColor Color = ActualDamage > 0.f ? FColor::Red : FColor::Green;
	DrawDebugSphere(GetWorld(), CenterLocation, SphereRadius, 16, Color, false, 1.f);
}

void APCRSerinDollHandCharacter::EnablePaperAttackCollision()
{
	PaperAttackSweepPlane->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

void APCRSerinDollHandCharacter::DisablePaperAttackCollision()
{
	PaperAttackSweepPlane->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
}

void APCRSerinDollHandCharacter::HandlePaperAttackSweepStart()
{
	PlayPaperAttackEffect();
	EnablePaperAttackCollision();
}

void APCRSerinDollHandCharacter::HandlePaperAttackSweepEnd()
{
	StopPaperAttackEffect();
	DisablePaperAttackCollision();
	PaperAttackData.AttackedActors.Reset();
}

void APCRSerinDollHandCharacter::HandlePaperAttackHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (PaperAttackData.AttackedActors.Find(OtherActor) == INDEX_NONE)
	{
		PaperAttackData.AttackedActors.AddUnique(OtherActor);
		const FDamageEvent DamageEvent;
		OtherActor->TakeDamage(PaperAttackData.Damage, DamageEvent, CachedSerinDollHead->GetController(), this);

		DrawDebugBox(GetWorld(), GetActorLocation(), PaperAttackSweepPlane->GetScaledBoxExtent(), FColor::Red, false, 1.f);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
											   SerinDollDataAsset->PaperAttackHitEffect,
											   OtherActor->GetActorLocation());
	}
}

void APCRSerinDollHandCharacter::EnableScissorsAttackCollision()
{
	ScissorsAttackHitPlane->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
}

void APCRSerinDollHandCharacter::DisableScissorsAttackCollision()
{
	ScissorsAttackHitPlane->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
}

void APCRSerinDollHandCharacter::HandleScissorsAttackOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ScissorsAttackData.AttackedActors.Find(OtherActor) == INDEX_NONE)
	{
		ScissorsAttackData.AttackedActors.AddUnique(OtherActor);
		const FDamageEvent DamageEvent;
		OtherActor->TakeDamage(ScissorsAttackData.Damage, DamageEvent, CachedSerinDollHead->GetController(), this);

		const FVector Direction = GetActorRotation().Vector();

		DrawDebugBox(GetWorld(), GetActorLocation() + Direction * 300.f, ScissorsAttackHitPlane->GetScaledBoxExtent(), GetActorRotation().Quaternion(), FColor::Red, false, 1.f);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
		                                               SerinDollDataAsset->ScissorsAttackHitEffect,
		                                               OtherActor->GetActorLocation());
	}
}

void APCRSerinDollHandCharacter::HandleScissorsAttackHitStart()
{
	EnableScissorsAttackCollision();
}

void APCRSerinDollHandCharacter::HandleScissorsAttackHitEnd()
{
	DisableScissorsAttackCollision();
	ScissorsAttackData.AttackedActors.Reset();
}

void APCRSerinDollHandCharacter::Pattern1Shoot()
{
	APCRSerinDollPattern1Projectile* Pattern1Projectile = GetWorld()->SpawnActor<APCRSerinDollPattern1Projectile>(APCRSerinDollPattern1Projectile::StaticClass());
	Pattern1Projectile->OnDetachedCard.BindUObject(CachedSerinDollHead, &APCRSerinDollHeadCharacter::Pattern1DetachCountCheck);
	CachedSerinDollHandAnimInstance->OnPattern1LastShoot.AddUObject(Pattern1Projectile, &APCRSerinDollPattern1Projectile::Pattern1ExplosionTimerStart);

	FVector StartLocation = GetActorLocation();
	StartLocation.Z = CachedSerinDollHead->CachedErica->GetActorLocation().Z;
	FVector TargetLocation;
	if (Pattern1Data.Pattern1TargetLocations.IsValidIndex(Pattern1Data.Pattern1TargetLocationIndex))
	{
		TargetLocation = Pattern1Data.Pattern1TargetLocations[Pattern1Data.Pattern1TargetLocationIndex];
		++Pattern1Data.Pattern1TargetLocationIndex;
	}
	else
	{
		TargetLocation = CachedSerinDollHead->CachedErica->GetActorLocation();
	}

	TargetLocation.Z = StartLocation.Z;
	Pattern1Projectile->Shoot(CachedSerinDollHead, StartLocation, TargetLocation);
}

void APCRSerinDollHandCharacter::HandlePatter2Start()
{
	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel6);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
}

void APCRSerinDollHandCharacter::HandlePattern2CardPull(APCREricaCardProjectile* AttachedCard)
{
	CachedSerinDollHead->HandlePattern2CardPull();
}

void APCRSerinDollHandCharacter::HandleToIdle()
{
	CurrentState = EState::Idle;
}

void APCRSerinDollHandCharacter::HandleRockAttackChaseEnded()
{
	RockAttackData.bIsChasing = false;
}
