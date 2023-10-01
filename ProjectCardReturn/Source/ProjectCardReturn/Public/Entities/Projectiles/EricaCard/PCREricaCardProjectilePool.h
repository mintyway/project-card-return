// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Projectiles/Base/PCRBaseProjectilePool.h"
#include "GameFramework/Actor.h"
#include "PCREricaCardProjectilePool.generated.h"

class APCREricaCardProjectile;

UCLASS()
class PROJECTCARDRETURN_API APCREricaCardProjectilePool : public APCRBaseProjectilePool
{
	GENERATED_BODY()
	
public:	
	APCREricaCardProjectilePool();
};
