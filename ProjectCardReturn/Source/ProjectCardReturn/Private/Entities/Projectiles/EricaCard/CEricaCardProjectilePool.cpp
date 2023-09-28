// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/EricaCard/CEricaCardProjectilePool.h"

#include "Entities/Projectiles/EricaCard/CEricaCardProjectile.h"
#include "Game/CParameterDataAsset.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACEricaCardProjectilePool::ACEricaCardProjectilePool()
{
	if (IsValid(GetParameterDataAsset()))
	{
		ProjectilePoolSize = GetParameterDataAsset()->EricaCardCount;
	}
}