// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "CBaseProjectile.generated.h"

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

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
};
