// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/Base/PCRBaseProjectilePool.h"

#include "Entities/Projectiles/Base/PCRBaseProjectile.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Game/PCRParameterDataAsset.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

APCRBaseProjectilePool::APCRBaseProjectilePool()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectilePoolSize = 100;
	ProjectilePoolSizeLimit = false;

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}
}

/**
 * 투사체를 생성하고 초기화합니다.
 * @param ProjectileClass 투사체 클래스
 * @param Shooter 투사체를 사용하는 액터
 */
void APCRBaseProjectilePool::InitProjectilePool(UClass* ProjectileClass, AActor* Shooter)
{
	RETURN_IF_INVALID(IsValid(GetWorld()));
	for (int32 i = 0; i < ProjectilePoolSize; ++i)
	{
		APCRBaseProjectile* NewProjectile = GetWorld()->SpawnActor<APCRBaseProjectile>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator);
		NewProjectile->Init(Shooter, this);
		ProjectilePool.Push(NewProjectile);
	}
}

/**
 * 풀로부터 투사체를 가져옵니다.
 * @param Location 투사체를 생성할 위치
 * @return 생성된 투사체 또는 nullptr
 */
APCRBaseProjectile* APCRBaseProjectilePool::GetProjectile(const FVector& Location)
{
	if (ProjectilePool.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("CardPool Is Empty!"));

		return nullptr;
	}

	APCRBaseProjectile* AvailableCard = ProjectilePool.Pop();
	RETURN_IF_INVALID(AvailableCard, nullptr);
	AvailableCard->SetActorHiddenInGame(false);
	AvailableCard->SetActorTickEnabled(true);
	AvailableCard->SetActorLocation(Location);
	AvailableCard->SetCollision(true);
	AvailableCard->GetProjectileMovementComponent()->Activate();
	return AvailableCard;
}

void APCRBaseProjectilePool::ReturnProjectile(APCRBaseProjectile* Projectile)
{
	ProjectilePool.Push(Projectile);
}
