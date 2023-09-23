// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Projectiles/CEricaCardProjectilePool.h"

#include "Entities/Projectiles/CEricaCardProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACEricaCardProjectilePool::ACEricaCardProjectilePool()
{
	ProjectilePoolSize = 30;
	UE_LOG(LogTemp, Warning, TEXT("%d"), ProjectilePoolSize);
}