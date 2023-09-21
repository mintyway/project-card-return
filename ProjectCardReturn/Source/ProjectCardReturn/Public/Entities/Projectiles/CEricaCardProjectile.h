// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Projectiles/CBaseProjectile.h"
#include "CEricaCardProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API ACEricaCardProjectile : public ACBaseProjectile
{
	GENERATED_BODY()

public:
	ACEricaCardProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:

private:
	
};
