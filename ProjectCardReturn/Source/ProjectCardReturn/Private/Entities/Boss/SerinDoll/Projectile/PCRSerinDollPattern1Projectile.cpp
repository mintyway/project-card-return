// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Projectile/PCRSerinDollPattern1Projectile.h"

#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "GameFramework/ProjectileMovementComponent.h"

APCRSerinDollPattern1Projectile::APCRSerinDollPattern1Projectile()
	: State(ESerinDollProjectileState::Unused), ProjectileSpeed(3000.f), Range(3000.f), bOnceDetached(false)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPCRSerinDollPrimaryDataAsset> DA_SerinDoll(TEXT("/Script/ProjectCardReturn.PCRSerinDollPrimaryDataAsset'/Game/DataAssets/DA_SerinDoll.DA_SerinDoll'"));
	if (DA_SerinDoll.Succeeded())
	{
		SerinDollDataAsset = DA_SerinDoll.Object;
	}

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	if (SphereComponent)
	{
		SetRootComponent(SphereComponent);
		SphereComponent->InitSphereRadius(30.f);
	}

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetupAttachment(SphereComponent);
		StaticMeshComponent->SetCollisionProfileName("NoCollision");
		StaticMeshComponent->SetStaticMesh(SerinDollDataAsset->Pattern1ProjectileMesh);
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.f;
		ProjectileMovementComponent->InitialSpeed = 0.f;
		ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	}
}

void APCRSerinDollPattern1Projectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DisableProjectile();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APCRSerinDollPattern1Projectile::HandleBossOverlap);
}

void APCRSerinDollPattern1Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
		case ESerinDollProjectileState::Unused:
		{
			return;
		}
		case ESerinDollProjectileState::Shooting:
		{
			HandleShooting();
			break;
		}
		case ESerinDollProjectileState::Stop:
		{
			break;
		}
		case ESerinDollProjectileState::Returning:
		{
			break;
		}
	}
}

void APCRSerinDollPattern1Projectile::Shoot(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction)
{
	SetOwner(NewOwner);
	LaunchLocation = StartLocation;

	SetActorLocationAndRotation(LaunchLocation, FRotationMatrix::MakeFromX(Direction).Rotator());
	SetActorHiddenInGame(false);
	EnableProjectile();
	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	ProjectileMovementComponent->Velocity = Direction.GetSafeNormal() * ProjectileSpeed;

	State = ESerinDollProjectileState::Shooting;
}

void APCRSerinDollPattern1Projectile::Release()
{
	Destroy();
}

void APCRSerinDollPattern1Projectile::BindOnReturnCardBegin(APCREricaCardProjectile* AttachedCard)
{
	AttachedCard->OnReturnCardBegin.AddUObject(this, &APCRSerinDollPattern1Projectile::HandleDetachedCard);
}

void APCRSerinDollPattern1Projectile::EnableProjectile()
{
	ProjectileMovementComponent->Activate();
	SetActorHiddenInGame(false);
	EnableCollisionDetection();
}

void APCRSerinDollPattern1Projectile::DisableProjectile()
{
	ProjectileMovementComponent->Deactivate();
	SetActorHiddenInGame(true);
	DisableCollisionDetection();
}

void APCRSerinDollPattern1Projectile::EnableCollisionDetection()
{
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel6);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block);
}

void APCRSerinDollPattern1Projectile::DisableCollisionDetection()
{
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void APCRSerinDollPattern1Projectile::HandleShooting()
{
	if (IsAtMaxRange())
	{
		EnableProjectile();
		HandleStop();
	}
}

bool APCRSerinDollPattern1Projectile::IsAtMaxRange()
{
	const float Distance = FVector::Dist(LaunchLocation, GetActorLocation());
	if (Distance >= Range)
	{
		return true;
	}

	return false;
}

void APCRSerinDollPattern1Projectile::HandleStop()
{
	ProjectileMovementComponent->Deactivate();

	State = ESerinDollProjectileState::Stop;
}

void APCRSerinDollPattern1Projectile::HandleDetachedCard(APCREricaCardProjectile* AttachedCard)
{
	if (!bOnceDetached)
	{
		const APCRSerinDollHeadCharacter* CachedSerinDollHead = Cast<APCRSerinDollHeadCharacter>(GetOwner());
		check(CachedSerinDollHead);

		const APCRLiftActor* CachedLift = CachedSerinDollHead->GetCachedLift();
		check(CachedLift);


		FVector TargetLocation;
		FVector Direction;
		const float Scala = 7000.f;
		if (CachedLift->IsOverlappedPattern1())
		{
			TargetLocation = CachedSerinDollHead->GetActorLocation();
			TargetLocation.Z = GetActorLocation().Z;
			Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		}
		else
		{
			TargetLocation = CachedSerinDollHead->GetCachedErica()->GetActorLocation();
			TargetLocation.Z = GetActorLocation().Z;
			Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
		}

		SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel9, ECR_Overlap);
		ProjectileMovementComponent->Activate();
		ProjectileMovementComponent->Velocity = Direction * Scala;

		if (OnDetachedCard.IsBound())
		{
			OnDetachedCard.Execute();
		}

		bOnceDetached = true;
	}
}

void APCRSerinDollPattern1Projectile::HandleBossOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	float DamageAmount;
	const FDamageEvent DamageEvent;

	if (APCREricaCharacter* Erica = Cast<APCREricaCharacter>(OtherActor))
	{
		DamageAmount = 20.f;
		OtherActor->TakeDamage(DamageAmount, DamageEvent, nullptr, this);
	}
	else
	{
		DamageAmount = 5.f;
		OtherActor->TakeDamage(DamageAmount, DamageEvent, nullptr, this);

		Destroy();
	}
}
