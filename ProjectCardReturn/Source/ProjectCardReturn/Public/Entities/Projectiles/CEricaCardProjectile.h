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
	virtual void Init(AActor* Shooter, AActor* Pool) override;

	void ShootCard(const FVector& Direction);
	void ReturnCard();
	bool IsShooting() const { return bIsShooting; }

private:
	void CardReturnMovement(float DeltaSeconds);
	void CheckCardRangeAndStop(float DeltaSeconds);
	
	float Speed;
	float ReturnSpeed;
	float Range;
	float ReturnRange;
	bool bIsShooting;
	bool bIsReturning;
};
