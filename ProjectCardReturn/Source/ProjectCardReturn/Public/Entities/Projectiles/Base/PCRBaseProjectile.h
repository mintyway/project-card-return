// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Game/PCRSoundPrimaryDataAsset.h"
#include "GameFramework/Actor.h"
#include "PCRBaseProjectile.generated.h"

class UPCRParameterDataAsset;
class UBoxComponent;
class UPCRProjectileDataAsset;
class UProjectileMovementComponent;
class UPCRSoundPrimaryDataAsset;

DECLARE_MULTICAST_DELEGATE(FLaunchProjectileSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FReleaseProjectileSignature, APCRBaseProjectile*);

/**
 * 기본 베이스 투사체 클래스입니다.
 */
UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRBaseProjectile();

protected:
	virtual void PostInitializeComponents() override;

public:
	virtual void LaunchProjectile(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction);
	virtual void ReleaseToProjectilePool();
	
	FLaunchProjectileSignature OnLaunchProjectile;
	FReleaseProjectileSignature OnReleaseProjectile;

protected:
	virtual void EnableProjectile();
	virtual void DisableProjectile();
	
	/**
	 * 서브클래스의 충돌판정 설정을 이 가상함수 내부에 정의하고 이를 통해 초기화, 재설정을 가능하도록 설계해야합니다. 오브젝트 풀에서 나올때 해당 함수가 호출됩니다.\n
	 * 이 함수는 순수가상함수입니다.
	 */
	virtual void EnableCollisionDetection() PURE_VIRTUAL();
	virtual void DisableCollisionDetection();

	UPROPERTY(VisibleAnywhere, Category = "Collider")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY()
	TObjectPtr<const UPCRProjectileDataAsset> ProjectileDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRSoundPrimaryDataAsset> SoundDataAsset;
	
	FVector ShootLocation;
	float ProjectileSpeed;
};
