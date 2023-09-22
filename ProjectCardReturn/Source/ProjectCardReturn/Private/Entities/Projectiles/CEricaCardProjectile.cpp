// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/CEricaCardProjectile.h"

#include "Entities/Projectiles/CEricaCardProjectilePool.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Entities/Projectiles/CProjectileDataAsset.h"

ACEricaCardProjectile::ACEricaCardProjectile()
{
	Speed = 3000.f;
	ReturnSpeed = 6000.f;
	Range = 1000.f;
	ReturnRange = 50.f;
	
	if (GetBoxComponent())
	{
		GetBoxComponent()->InitBoxExtent(FVector(4.4, 3.1, 1.0));
		GetBoxComponent()->SetRelativeScale3D(FVector(7.0, 7.0, 7.0));
	}
	
	if (GetStaticMeshComponent() && GetProjectileDataAsset())
	{
		GetStaticMeshComponent()->SetStaticMesh(GetProjectileDataAsset()->GetEricaCardMesh());
	}

	if (GetProjectileMovementComponent())
	{
		GetProjectileMovementComponent()->InitialSpeed = 0;
		GetProjectileMovementComponent()->MaxSpeed = Speed;
	}
}

void ACEricaCardProjectile::Init(AActor* Shooter, AActor* Pool)
{
	Super::Init(Shooter, Pool);

	bIsShooting = false;
	bIsReturning = false;
}

void ACEricaCardProjectile::BeginPlay()
{
	Super::BeginPlay();

	RETURN_IF_INVALID(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("Owner Address: %s"), *GetOwner()->GetName());
}

void ACEricaCardProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsReturning)
	{
		CardReturnMovement(DeltaSeconds);
	}
	else
	{
		CheckCardRangeAndStop(DeltaSeconds);
	}

	// UE_LOG(LogTemp, Warning, TEXT("%f"), GetProjectileMovementComponent()->Velocity.Size());
}

/**
 * 카드를 발사시킵니다.
 * @param Direction 카드의 진행방향
 */
void ACEricaCardProjectile::ShootCard(const FVector& Direction)
{
	GetProjectileMovementComponent()->Velocity = Direction.GetSafeNormal() * Speed;
	bIsShooting = true;
}

void ACEricaCardProjectile::ReturnCard()
{
	SIMPLE_LOG;

	bIsReturning = true;
	SetActorTickEnabled(true);
	GetProjectileMovementComponent()->Activate();
}

/**
 * 카드 플레이어 방향으로 이동시키고 플레이어에 닿을 시 풀로 반환합니다.
 */
void ACEricaCardProjectile::CardReturnMovement(float DeltaSeconds)
{
	RETURN_IF_INVALID(IsValid(GetOwner()));
	FVector MoveDirection = (GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector NewLocation = GetActorLocation() + MoveDirection * ReturnSpeed * DeltaSeconds;
		
	SetActorLocation(NewLocation);

	float ShooterDistance = FVector::DistSquared(GetOwner()->GetActorLocation(), GetActorLocation());
	if (ShooterDistance <= (ReturnRange * ReturnRange))
	{
		bIsReturning = false;
		ACEricaCardProjectilePool* Pool = Cast<ACEricaCardProjectilePool>(GetOwnerPool());
		Pool->ReturnCard(this);
	}
}

/**
 * 카드를 쏜 지점부터 특정 거리에 도달할 시 카드를 멈춥니다.
 */
void ACEricaCardProjectile::CheckCardRangeAndStop(float DeltaSeconds)
{
	float Distance = FVector::DistSquared(GetShootLocation(), GetActorLocation());
	if (Distance >= (Range * Range))
	{
		GetProjectileMovementComponent()->Deactivate();
		SetActorTickEnabled(false);
		bIsShooting = false;
	}
}
