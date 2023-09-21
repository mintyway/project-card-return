// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/CEricaCardProjectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Entities/Projectiles/CProjectileDataAsset.h"

ACEricaCardProjectile::ACEricaCardProjectile()
{
	if (GetBoxComponent())
	{
		GetBoxComponent()->InitBoxExtent(FVector(4.4, 3.1, 1.0));
		GetBoxComponent()->SetRelativeScale3D(FVector(3.0, 3.0, 3.0));
	}
	
	if (GetStaticMeshComponent() && GetProjectileDataAsset())
	{
		GetStaticMeshComponent()->SetStaticMesh(GetProjectileDataAsset()->GetEricaCardMesh());
	}

	if (GetProjectileMovementComponent())
	{
		GetProjectileMovementComponent()->InitialSpeed = 2000.f;
		GetProjectileMovementComponent()->MaxSpeed = 2000.f;
	}
}

void ACEricaCardProjectile::Init(AActor* InitShooter)
{
	Super::Init(InitShooter);

	bIsShooting = true;
	bIsReturning = false;
}

void ACEricaCardProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ACEricaCardProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float Distance = static_cast<float>(FVector::DistSquared(GetStartLocation(), GetActorLocation()));
	if (!bIsReturning && (Distance >= (1000.f * 1000.f)))
	{
		GetProjectileMovementComponent()->Deactivate();

		SetActorTickEnabled(false);
		bIsShooting = false;
	}

	if (bIsReturning)
	{
		FVector MoveDirection = (GetShooter()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = GetActorLocation() + MoveDirection * 2000.f * DeltaSeconds;
		
		SetActorLocation(NewLocation);

		float ShooterDistance = FVector::DistSquared(GetShooter()->GetActorLocation(), GetActorLocation());
		if (ShooterDistance <= (10.f * 10.f))
		{
			Destroy();
		}
	}

	// UE_LOG(LogTemp, Warning, TEXT("%f"), GetProjectileMovementComponent()->Velocity.Size());
}

bool ACEricaCardProjectile::IsShooting()
{
	return bIsShooting;
}

void ACEricaCardProjectile::CardReturn()
{
	SIMPLE_LOG;

	bIsReturning = true;
	SetActorTickEnabled(true);
	GetProjectileMovementComponent()->Velocity = FVector(100.0, 0.0, 0.0);
	GetProjectileMovementComponent()->Activate();
}
