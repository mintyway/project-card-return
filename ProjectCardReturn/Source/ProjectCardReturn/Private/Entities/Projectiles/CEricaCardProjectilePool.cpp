// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/CEricaCardProjectilePool.h"

#include "Entities/Projectiles/CEricaCardProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACEricaCardProjectilePool::ACEricaCardProjectilePool()
{
	PrimaryActorTick.bCanEverTick = true;
	CardPoolSize = 30;
}

void ACEricaCardProjectilePool::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RETURN_IF_INVALID(IsValid(GetWorld()));
	for (int32 i = 0; i < CardPoolSize; ++i)
	{
		ACEricaCardProjectile* NewCard = GetWorld()->SpawnActor<ACEricaCardProjectile>(FVector::ZeroVector, FRotator::ZeroRotator);
		NewCard->SetActorHiddenInGame(true);
		NewCard->SetActorTickEnabled(false);
		NewCard->GetProjectileMovementComponent()->Deactivate();
		CardStack.Push(NewCard);
	}
}

/**
 * 카드를 사용하기 위해 일괄적으로 초기화합니다.
 * @param Shooter 카드를 사용하는 액터입니다.
 */
void ACEricaCardProjectilePool::InitCard(AActor* Shooter)
{
	for (const auto& Card : CardStack)
	{
		Card->Init(Shooter, this);
	}
}

/**
 * 풀로부터 카드를 가져오고 이와 동시에 발사합니다.
 * @param Location 카드를 생성할 위치
 * @param Rotation 생성된 카드가 바라볼 방향
 * @return 생성된 카드
 */
TObjectPtr<ACEricaCardProjectile> ACEricaCardProjectilePool::GetCard(const FVector& Location, const FRotator& Rotation)
{
	if (CardStack.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CardPool Is Empty!"));

		return nullptr;
	}
	
	ACEricaCardProjectile* AvailableCard = CardStack.Pop();
	AvailableCard->SetActorHiddenInGame(false);
	AvailableCard->SetActorTickEnabled(true);
	AvailableCard->SetShootLocation(Location);
	AvailableCard->SetActorLocationAndRotation(Location, Rotation);
	AvailableCard->GetProjectileMovementComponent()->Activate();
	AvailableCard->ShootCard(Rotation.Vector());
	return AvailableCard;
}

void ACEricaCardProjectilePool::ReturnCard(ACEricaCardProjectile* Projectile)
{
	Projectile->SetActorHiddenInGame(true);
	Projectile->SetActorTickEnabled(false);
	Projectile->GetProjectileMovementComponent()->Deactivate();
	CardStack.Push(Projectile);
}
