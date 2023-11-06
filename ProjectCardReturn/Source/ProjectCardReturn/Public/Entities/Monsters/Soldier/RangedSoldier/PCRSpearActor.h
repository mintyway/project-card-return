// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "PCRSpearActor.generated.h"

class UPCRParameterDataAsset;
class UProjectileMovementComponent;
class UBoxComponent;
class UPCRMonsterDataAsset;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogSpearActor, Log, All);

DECLARE_MULTICAST_DELEGATE(FDetachedSpearSignature);
DECLARE_MULTICAST_DELEGATE(FDestroyedSpearSigniture);

UCLASS()
class PROJECTCARDRETURN_API APCRSpearActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APCRSpearActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void Throw(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction);
	
private:
	void DelayedDestroy();
	void DestroyTimerCallback();

	UFUNCTION()
	void HandleSpearHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;
};
