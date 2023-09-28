// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/EricaCard/CEricaCardProjectile.h"

#include "Entities/Projectiles/EricaCard/CEricaCardProjectilePool.h"
#include "Entities/Players/Erica/CEricaCharacter.h"
#include "Entities/Projectiles/Base/CProjectileDataAsset.h"
#include "Game/CParameterDataAsset.h"

#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"

ACEricaCardProjectile::ACEricaCardProjectile()
{
	if (IsValid(GetParameterDataAsset()))
	{
		ProjectileSpeed = GetParameterDataAsset()->EricaCardSpeed;
		ReturnSpeed = GetParameterDataAsset()->EricaCardReturnSpeed;
		Range = GetParameterDataAsset()->EricaCardRapidShotRange;
		ReturnRange = GetParameterDataAsset()->EricaCardReturnRange;
	}
	
	if (IsValid(GetBoxComponent()))
	{
		GetBoxComponent()->InitBoxExtent(FVector(4.4, 3.1, 1.0));
		GetBoxComponent()->SetRelativeScale3D(FVector(7.0, 7.0, 1.0));
	}

	if (IsValid(GetStaticMeshComponent()) && IsValid(GetProjectileDataAsset()))
	{
		GetStaticMeshComponent()->SetStaticMesh(GetProjectileDataAsset()->GetEricaCardMesh());
	}

	if (IsValid(GetProjectileMovementComponent()))
	{
		GetProjectileMovementComponent()->MaxSpeed = ProjectileSpeed;
	}
}

void ACEricaCardProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnActorBeginOverlap.AddDynamic(this, &ACEricaCardProjectile::HandleBeginOverlap);
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

void ACEricaCardProjectile::Init(AActor* Shooter, ACBaseProjectilePool* Pool)
{
	Super::Init(Shooter, Pool);

	bIsShooting = false;
	bIsReturning = false;
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
		SetCollision(true);
	}
	else
	{
		SetActorTickEnabled(false);
		GetProjectileMovementComponent()->Deactivate();
		SetCollision(false);
	}
}

/**
 * 카드를 플레이어에게로 복귀하는 상태로 바꿉니다.
 */
void ACEricaCardProjectile::ReturnCard()
{
	bIsReturning = true;
	SetCardEnable(true);
	
	OnReturnCardBegin.Broadcast();
}

void ACEricaCardProjectile::HandleBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	RETURN_IF_INVALID(IsValid(OtherCharacter));

	const FVector CurrentDirection = OverlappedActor->GetActorForwardVector();
	const FVector CurrentOtherActorDirection = OtherCharacter->GetActorForwardVector();

	const float DotResult = FVector::DotProduct(CurrentDirection, CurrentOtherActorDirection);
	if (DotResult < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("정면 충돌"));
		// TODO: 현재는 해당 액터가 밀리면서 여러번 오버랩될 수 있는 상황입니다. 나중에 충돌 당하는 액터에게 오버랩 쿨타임을 구현해 이를 막아야합니다.
		OtherCharacter->LaunchCharacter(OverlappedActor->GetActorForwardVector() * 500.f, false, false);
		
		ACEricaCharacter* EricaCharacter = Cast<ACEricaCharacter>(GetOwner());
		const FDamageEvent DamageEvent;
		OtherActor->TakeDamage(EricaCharacter->GetAttackPower() / 5, DamageEvent, EricaCharacter->GetController(), EricaCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("후면 충돌"));
		ACEricaCharacter* EricaCharacter = Cast<ACEricaCharacter>(GetOwner());
		const FDamageEvent DamageEvent;
		OtherActor->TakeDamage(EricaCharacter->GetAttackPower(), DamageEvent, EricaCharacter->GetController(), EricaCharacter);
	}
}

/**
 * 카드 플레이어 방향으로 이동시키고 플레이어에 닿을 시 풀로 반환합니다.
 */
void ACEricaCardProjectile::CardReturnMovement(float DeltaSeconds)
{
	RETURN_IF_INVALID(IsValid(GetOwner()));
	const FVector MoveDirection = (GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	// GetProjectileMovementComponent()->Velocity = MoveDirection * ReturnSpeed;
	// SetActorRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	SetActorLocationAndRotation(GetActorLocation() + (MoveDirection * ReturnSpeed * DeltaSeconds), MoveDirection.Rotation());

	const float ShooterDistance = FVector::DistSquared(GetOwner()->GetActorLocation(), GetActorLocation());
	if (ShooterDistance <= (ReturnRange * ReturnRange))
	{
		bIsReturning = false;
		ReturnToProjectilePool();
		
		OnReturnCardEnd.Broadcast();
	}
}

/**
 * 카드를 쏜 지점부터 특정 거리에 도달할 시 카드를 멈춥니다.
 */
void ACEricaCardProjectile::CheckCardRangeAndStop(float DeltaSeconds)
{
	const float Distance = FVector::DistSquared(ShootLocation, GetActorLocation());
	if (Distance >= (Range * Range))
	{
		bIsShooting = false;
		SetCardEnable(false);
	}
}
