// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/EricaCard/PCREricaCardProjectilePool.h"

#include "Entities/Projectiles/EricaCard/PCREricaCardProjectile.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/ProjectileMovementComponent.h"

APCREricaCardProjectilePool::APCREricaCardProjectilePool()
{
	if (IsValid(GetParameterDataAsset()))
	{
		ProjectilePoolSize = GetParameterDataAsset()->EricaCardCount;
	}
}