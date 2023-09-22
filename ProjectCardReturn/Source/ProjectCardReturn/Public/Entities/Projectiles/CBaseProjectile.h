// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "CBaseProjectile.generated.h"

class UBoxComponent;
class UCProjectileDataAsset;
class UProjectileMovementComponent;
class ACBaseProjectilePool;

/**
 * 기본 베이스 투사체 클래스입니다.
 */
UCLASS()
class PROJECTCARDRETURN_API ACBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBaseProjectile();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Init(AActor* Shooter, ACBaseProjectilePool* Pool);
	virtual void Shoot(const FVector& Direction);
	void ReturnToProjectilePool();
	
	FORCEINLINE UBoxComponent* GetBoxComponent() const { return BoxComponent; }
	FORCEINLINE UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }
	FORCEINLINE ACBaseProjectilePool* GetOwnerPool() const { return OwnerPool; }
	FORCEINLINE UCProjectileDataAsset* GetProjectileDataAsset() const { return ProjectileDataAsset; }

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
	TObjectPtr<ACBaseProjectilePool> OwnerPool;

	TObjectPtr<UCProjectileDataAsset> ProjectileDataAsset;
};
