// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "CBaseProjectile.generated.h"

class UBoxComponent;
class UCProjectileDataAsset;
class UProjectileMovementComponent;
class ACEricaCardProjectilePool;

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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Init(AActor* Shooter, AActor* Pool = nullptr);
	
	FORCEINLINE TObjectPtr<UBoxComponent> GetBoxComponent() const { return BoxComponent; }
	FORCEINLINE TObjectPtr<UProjectileMovementComponent> GetProjectileMovementComponent() const { return ProjectileMovementComponent; }
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetStaticMeshComponent() const { return StaticMeshComponent; }
	FORCEINLINE TObjectPtr<UCProjectileDataAsset> GetProjectileDataAsset() const { return ProjectileDataAsset; }
	FORCEINLINE FVector GetShootLocation() const { return ShootLocation; }
	FORCEINLINE void SetShootLocation(const FVector& NewLocation) { ShootLocation = NewLocation; }
	FORCEINLINE TObjectPtr<AActor> GetOwnerPool() const { return OwnerPool; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	TObjectPtr<UCProjectileDataAsset> ProjectileDataAsset;
	FVector ShootLocation;

	TObjectPtr<AActor> OwnerPool;
};
