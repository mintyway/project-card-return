// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Projectile/PCRSerinDollPattern1Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

APCRSerinDollPattern1Projectile::APCRSerinDollPattern1Projectile()
	: State(ESerinDollProjectileState::Unused), ProjectileSpeed(1500.f), Range(1000.f)
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
	ProjectileMovementComponent->Velocity = Direction.GetSafeNormal() * ProjectileSpeed;

	State = ESerinDollProjectileState::Shooting;
}

void APCRSerinDollPattern1Projectile::Release()
{
	Destroy();
}

void APCRSerinDollPattern1Projectile::BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard) {}

void APCRSerinDollPattern1Projectile::EnableProjectile()
{
	SetActorHiddenInGame(false);
	EnableCollisionDetection();
}

void APCRSerinDollPattern1Projectile::DisableProjectile()
{
	SetActorHiddenInGame(true);
	DisableCollisionDetection();
}

void APCRSerinDollPattern1Projectile::EnableCollisionDetection()
{
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel6);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
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
	
	State = ESerinDollProjectileState::Stop;
}
