// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/EricaCard/PCREricaCardProjectilePool.h"

#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogEricaCardProjectilePool);

APCREricaCardProjectilePool::APCREricaCardProjectilePool()
{
	if (ParameterDataAsset)
	{
		ProjectilePoolSize = ParameterDataAsset->EricaCardCount;
	}
}

APCRBaseProjectile* APCREricaCardProjectilePool::HandleEmptyPool()
{
	UE_LOG(PCRLogEricaCardProjectilePool, Log, TEXT("카드가 모두 소진되었습니다."));
	
	return Super::HandleEmptyPool();
}
