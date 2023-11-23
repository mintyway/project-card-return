// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/Base/PCRBaseProjectilePool.h"

#include "Entities/Projectiles/Base/PCRBaseProjectile.h"
#include "Game/PCRParameterDataAsset.h"

DEFINE_LOG_CATEGORY(PCRLogBaseProjectilePool);

APCRBaseProjectilePool::APCRBaseProjectilePool()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectilePoolSize = 100;

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}
}

/**
 * 투사체를 생성하고 초기화합니다.
 * @param ProjectileClass 투사체 클래스
 */
void APCRBaseProjectilePool::InitProjectilePool(UClass* ProjectileClass)
{
	for (int32 i = 0; i < ProjectilePoolSize; ++i)
	{
		APCRBaseProjectile* NewProjectile = GetWorld()->SpawnActor<APCRBaseProjectile>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator);
		FName FolderName = TEXT("Projectiles");
		ProjectilePool.Enqueue(NewProjectile);
	}
}

void APCRBaseProjectilePool::Add(UClass* ProjectileClass, int32 InCount)
{
	for (int32 i = 0; i < InCount; ++i)
	{
		APCRBaseProjectile* NewProjectile = GetWorld()->SpawnActor<APCRBaseProjectile>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator);
		FName FolderName = TEXT("Projectiles");
		ProjectilePool.Enqueue(NewProjectile);
	}
}

/**
 * 풀로부터 투사체를 가져옵니다.
 * @return 생성된 투사체 또는 nullptr
 */
APCRBaseProjectile* APCRBaseProjectilePool::Acquire()
{
	TObjectPtr<APCRBaseProjectile> AcquireProjectile;
	if (ProjectilePool.Dequeue(AcquireProjectile))
	{
		const FDelegateHandle NewHandle = AcquireProjectile->OnReleaseProjectile.AddUObject(this, &APCRBaseProjectilePool::Release);
		OnReleaseProjectileDelegateHandleMap.Add(AcquireProjectile, NewHandle);
		return AcquireProjectile;
	}
	else
	{
		return HandleEmptyPool();
	}
}

/**
 * 풀로 투사체를 반환합니다. 이 함수는 델리게이트로 호출합니다.
 * @param Projectile 풀로 반환할 투사체
 */
void APCRBaseProjectilePool::Release(APCRBaseProjectile* Projectile)
{
	ProjectilePool.Enqueue(Projectile);
	const FDelegateHandle* ExistingHandle = OnReleaseProjectileDelegateHandleMap.Find(Projectile);
	if (ExistingHandle)
	{
		Projectile->OnReleaseProjectile.Remove(*ExistingHandle);
	}
}

APCRBaseProjectile* APCRBaseProjectilePool::HandleEmptyPool()
{
	return nullptr;
}
