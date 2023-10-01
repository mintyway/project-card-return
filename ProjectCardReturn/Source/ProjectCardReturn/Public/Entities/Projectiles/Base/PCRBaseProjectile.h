// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRBaseProjectile.generated.h"

class UPCRParameterDataAsset;
class UBoxComponent;
class UPCRProjectileDataAsset;
class UProjectileMovementComponent;
class APCRBaseProjectilePool;

DECLARE_MULTICAST_DELEGATE(FOnShootCardDelegate);

/**
 * 기본 베이스 투사체 클래스입니다.
 */
UCLASS()
class PROJECTCARDRETURN_API APCRBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRBaseProjectile();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Init(AActor* Shooter, APCRBaseProjectilePool* Pool);
	virtual void Shoot(const FVector& Direction);
	virtual void SetCollision(bool bIsEnable);
	void ReturnToProjectilePool();
	
	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComponent; }
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }
	FORCEINLINE APCRBaseProjectilePool* GetOwnerPool() const { return OwnerPool; }
	
	FORCEINLINE const UPCRProjectileDataAsset* GetProjectileDataAsset() const { return ProjectileDataAsset; }
	FORCEINLINE const UPCRParameterDataAsset* GetParameterDataAsset() const { return ParameterDataAsset; }

	FOnShootCardDelegate OnShootCard;

protected:
	FVector ShootLocation;
	float ProjectileSpeed;

private:
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY()
	TObjectPtr<APCRBaseProjectilePool> OwnerPool;

	TObjectPtr<const UPCRProjectileDataAsset> ProjectileDataAsset;
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;
};
