// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Projectiles/Base/PCRBaseProjectile.h"
#include "PCREricaCardProjectile.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnReturnCardBeginDelegate);
DECLARE_MULTICAST_DELEGATE(FOnReturnCardEndDelegate);

DECLARE_LOG_CATEGORY_EXTERN(PCRLogEricaCardProjectile, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCREricaCardProjectile : public APCRBaseProjectile
{
	GENERATED_BODY()

public:
	APCREricaCardProjectile();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void Init(AActor* Shooter, APCRBaseProjectilePool* Pool) override;
	virtual void Shoot(const FVector& Direction) override;

	FOnReturnCardBeginDelegate OnReturnCardBegin;
	FOnReturnCardEndDelegate OnReturnCardEnd;

	void ReturnCard();
	void SetCardEnable(bool bIsEnable = true);
	bool IsShooting() const { return bIsShooting; }
	void SetRange(float NewRange) { Range = NewRange; }

private:
	UFUNCTION()
	void HandleBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void CardReturnMovement(float DeltaSeconds);
	void CheckCardRangeAndStop(float DeltaSeconds);

	float ReturnSpeed;
	float Range;
	float ReturnRange;
	bool bIsShooting;
	bool bIsReturning;
};
