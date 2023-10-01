// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRBaseProjectilePool.generated.h"

class UPCRParameterDataAsset;
class APCRBaseProjectile;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogBaseProjectilePool, Log, All);

UCLASS()
class PROJECTCARDRETURN_API APCRBaseProjectilePool : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRBaseProjectilePool();

public:
	virtual void InitProjectilePool(UClass* ProjectileClass, AActor* Shooter);
	virtual APCRBaseProjectile* GetProjectile(const FVector& Location);
	virtual void ReturnProjectile(APCRBaseProjectile* Projectile);

	FORCEINLINE TObjectPtr<UPCRParameterDataAsset> GetParameterDataAsset() const { return ParameterDataAsset; }

protected:
	TArray<TObjectPtr<APCRBaseProjectile>> ProjectilePool;
	int32 ProjectilePoolSize;
	bool ProjectilePoolSizeLimit;
	
private:
	TObjectPtr<UPCRParameterDataAsset> ParameterDataAsset;
};
