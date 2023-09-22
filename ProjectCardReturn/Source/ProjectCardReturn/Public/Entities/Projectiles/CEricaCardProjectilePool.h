// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "CEricaCardProjectilePool.generated.h"

class ACEricaCardProjectile;

// TODO: 카드 사용 시, 회수 시, 잔량 없을 시 델리게이트 필요
UCLASS()
class PROJECTCARDRETURN_API ACEricaCardProjectilePool : public AActor
{
	GENERATED_BODY()
	
public:	
	ACEricaCardProjectilePool();

protected:
	virtual void PostInitializeComponents() override;
	
public:
	void InitCard(AActor* Shooter);
	TObjectPtr<ACEricaCardProjectile> GetCard(const FVector& Location, const FRotator& Rotation);
	void ReturnCard(ACEricaCardProjectile* Projectile);

private:
	int32 CardPoolSize;
	TArray<TObjectPtr<ACEricaCardProjectile>> CardStack;
};
