// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Projectiles/Base/CBaseProjectilePool.h"
#include "GameFramework/Actor.h"
#include "CEricaCardProjectilePool.generated.h"

class ACEricaCardProjectile;

UCLASS()
class PROJECTCARDRETURN_API ACEricaCardProjectilePool : public ACBaseProjectilePool
{
	GENERATED_BODY()
	
public:	
	ACEricaCardProjectilePool();
};
