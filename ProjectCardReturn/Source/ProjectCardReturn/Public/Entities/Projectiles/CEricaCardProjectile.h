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
	virtual void Init(AActor* Shooter, ACBaseProjectilePool* Pool) override;
	virtual void Shoot(const FVector& Direction) override;

	void ReturnCard();
	void SetCardEnable(bool bIsEnable = true);
	bool IsShooting() const { return bIsShooting; }

private:
	void CardReturnMovement(float DeltaSeconds);
	void CheckCardRangeAndStop(float DeltaSeconds);
	
	float ReturnSpeed;
	float Range;
	float ReturnRange;
	bool bIsShooting;
	bool bIsReturning;
};
