// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PCREricaCardInteractable.h"
#include "PCRSerinDollPattern1Projectile.generated.h"

class USphereComponent;
class UPCRSerinDollPrimaryDataAsset;
class UPCRSoundPrimaryDataAsset;
class UProjectileMovementComponent;

UENUM()
enum class ESerinDollProjectileState
{
	Unused,
	Shooting,
	Stop,
	Returning
};

UCLASS()
class PROJECTCARDRETURN_API APCRSerinDollPattern1Projectile : public AActor, public IPCREricaCardInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APCRSerinDollPattern1Projectile();

protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

public:
	void Shoot(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction);
	void Release();

public:
	void BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard) override;

private: // 내부 함수
	void EnableProjectile();
	void DisableProjectile();
	
	void EnableCollisionDetection();
	void DisableCollisionDetection();

	void HandleShooting();
	
	bool IsAtMaxRange();
	void HandleStop();

private: // 컴포넌트
	UPROPERTY()
	TObjectPtr<const UPCRSerinDollPrimaryDataAsset> SerinDollDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRSoundPrimaryDataAsset> SoundDataAsset;
	
private: // 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Collider")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

private: // 데이터
	ESerinDollProjectileState State;
	FVector LaunchLocation;
	float ProjectileSpeed;
	float Range;
};
