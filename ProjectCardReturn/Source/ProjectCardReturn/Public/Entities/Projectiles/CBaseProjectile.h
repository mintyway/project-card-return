// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "CBaseProjectile.generated.h"

class UBoxComponent;
class UCProjectileDataAsset;
class UProjectileMovementComponent;

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
	virtual void Init(AActor* InitShooter);
	
	FORCEINLINE TObjectPtr<UBoxComponent> GetBoxComponent() const { return BoxComponent; }
	FORCEINLINE TObjectPtr<UProjectileMovementComponent> GetProjectileMovementComponent() const { return ProjectileMovementComponent; }
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetStaticMeshComponent() const { return StaticMeshComponent; }
	FORCEINLINE TObjectPtr<UCProjectileDataAsset> GetProjectileDataAsset() const { return ProjectileDataAsset; }
	FORCEINLINE TObjectPtr<AActor> GetShooter() const { return Shooter; }
	FORCEINLINE FVector GetStartLocation() const { return StartLocation; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<AActor> Shooter;

	TObjectPtr<UCProjectileDataAsset> ProjectileDataAsset;
	FVector StartLocation;
};
