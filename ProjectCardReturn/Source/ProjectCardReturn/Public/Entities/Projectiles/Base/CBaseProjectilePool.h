// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "CBaseProjectilePool.generated.h"

class UCParameterDataAsset;
class ACBaseProjectile;
UCLASS()
class PROJECTCARDRETURN_API ACBaseProjectilePool : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBaseProjectilePool();

public:
	virtual void InitProjectilePool(UClass* ProjectileClass, AActor* Shooter);
	virtual ACBaseProjectile* GetProjectile(const FVector& Location);
	virtual void ReturnProjectile(ACBaseProjectile* Projectile);

	FORCEINLINE TObjectPtr<UCParameterDataAsset> GetParameterDataAsset() const { return ParameterDataAsset; }

protected:
	TArray<TObjectPtr<ACBaseProjectile>> ProjectilePool;
	int32 ProjectilePoolSize;
	bool ProjectilePoolSizeLimit;
	
private:
	TObjectPtr<UCParameterDataAsset> ParameterDataAsset;
};
