// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Projectiles/CBaseProjectile.h"
#include "CEricaCardProjectile.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnReturnCardBeginDelegate);
DECLARE_MULTICAST_DELEGATE(FOnReturnCardEndDelegate);

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
	UFUNCTION()
	void ActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void Init(AActor* Shooter, ACBaseProjectilePool* Pool) override;
	virtual void Shoot(const FVector& Direction) override;

	FOnReturnCardBeginDelegate OnReturnCardBegin;
	FOnReturnCardEndDelegate OnReturnCardEnd;

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
