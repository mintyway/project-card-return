// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/CEricaCardProjectile.h"

#include "Entities/Projectiles/CEricaCardProjectilePool.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Entities/Projectiles/CProjectileDataAsset.h"

ACEricaCardProjectile::ACEricaCardProjectile()
{
	ProjectileSpeed = 3000.f;
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
		GetProjectileMovementComponent()->MaxSpeed = ProjectileSpeed;
	}
}

void ACEricaCardProjectile::Init(AActor* Shooter, ACBaseProjectilePool* Pool)
{
	Super::Init(Shooter, Pool);

	bIsShooting = false;
	bIsReturning = false;
}

void ACEricaCardProjectile::BeginPlay()
{
	Super::BeginPlay();
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
}

/**
 * 카드를 발사시킵니다.
 * @param Direction 카드의 진행방향
 */
void ACEricaCardProjectile::Shoot(const FVector& Direction)
{
	Super::Shoot(Direction);
	
	bIsShooting = true;
}

/**
 * 카드의 활성화 여부를 설정합니다.
 */
void ACEricaCardProjectile::SetCardEnable(bool bIsEnable)
{
	if (bIsEnable)
	{
		SetActorTickEnabled(true);
		GetProjectileMovementComponent()->Activate();
	}
	else
	{
		SetActorTickEnabled(false);
		GetProjectileMovementComponent()->Deactivate();
	}
}

/**
 * 카드를 플레이어에게로 복귀하는 상태로 바꿉니다.
 */
void ACEricaCardProjectile::ReturnCard()
{
	bIsReturning = true;
	SetCardEnable(true);
}

/**
 * 카드 플레이어 방향으로 이동시키고 플레이어에 닿을 시 풀로 반환합니다.
 */
void ACEricaCardProjectile::CardReturnMovement(float DeltaSeconds)
{
	RETURN_IF_INVALID(IsValid(GetOwner()));
	FVector MoveDirection = (GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	
	FVector NewLocation = GetActorLocation() + (MoveDirection * ReturnSpeed * DeltaSeconds);
	SetActorLocationAndRotation(NewLocation, FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	
	// GetProjectileMovementComponent()->Velocity = MoveDirection * ReturnSpeed;
	// SetActorRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	
	UE_LOG(LogTemp, Warning, TEXT("%f"), GetProjectileMovementComponent()->Velocity.Size());
	float ShooterDistance = FVector::DistSquared(GetOwner()->GetActorLocation(), GetActorLocation());
	if (ShooterDistance <= (ReturnRange * ReturnRange))
	{
		bIsReturning = false;
		ACEricaCardProjectilePool* Pool = Cast<ACEricaCardProjectilePool>(GetOwnerPool());
		ReturnToProjectilePool();
	}
}

/**
 * 카드를 쏜 지점부터 특정 거리에 도달할 시 카드를 멈춥니다.
 */
void ACEricaCardProjectile::CheckCardRangeAndStop(float DeltaSeconds)
{
	float Distance = FVector::DistSquared(ShootLocation, GetActorLocation());
	if (Distance >= (Range * Range))
	{
		bIsShooting = false;
		SetCardEnable(false);
	}
}
