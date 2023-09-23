// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/CBaseProjectilePool.h"

#include "Entities/Projectiles/CBaseProjectile.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACBaseProjectilePool::ACBaseProjectilePool()
	: ProjectilePoolSize(100), ProjectilePoolSizeLimit(false)
{
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("%d"), ProjectilePoolSize);
}

/**
 * 투사체를 생성하고 초기화합니다.
 * @param ProjectileClass 투사체 클래스
 * @param Shooter 투사체를 사용하는 액터
 */
void ACBaseProjectilePool::InitProjectilePool(UClass* ProjectileClass, AActor* Shooter)
{
	RETURN_IF_INVALID(IsValid(GetWorld()));
	for (int32 i = 0; i < ProjectilePoolSize; ++i)
	{
		auto test = ACBaseProjectile::StaticClass();
		ACBaseProjectile* NewProjectile = GetWorld()->SpawnActor<ACBaseProjectile>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator);
		NewProjectile->Init(Shooter, this);
		ProjectilePool.Push(NewProjectile);
	}
}

/**
 * 풀로부터 투사체를 가져옵니다.
 * @param Location 투사체를 생성할 위치
 * @return 생성된 투사체 또는 nullptr
 */
ACBaseProjectile* ACBaseProjectilePool::GetProjectile(const FVector& Location)
{
	if (ProjectilePool.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("CardPool Is Empty!"));

		return nullptr;
	}

	ACBaseProjectile* AvailableCard = ProjectilePool.Pop();
	RETURN_IF_INVALID(AvailableCard, nullptr);
	AvailableCard->SetActorHiddenInGame(false);
	AvailableCard->SetActorTickEnabled(true);
	AvailableCard->SetActorLocation(Location);
	AvailableCard->SetCollision();
	AvailableCard->GetProjectileMovementComponent()->Activate();
	return AvailableCard;
}

void ACBaseProjectilePool::ReturnProjectile(ACBaseProjectile* Projectile)
{
	ProjectilePool.Push(Projectile);
}
