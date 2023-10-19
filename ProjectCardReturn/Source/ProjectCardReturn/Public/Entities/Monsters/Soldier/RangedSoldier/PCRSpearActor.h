// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PCREricaCardInteractable.h"
#include "PCRSpearActor.generated.h"

class UPCRParameterDataAsset;
class UProjectileMovementComponent;
class UBoxComponent;
class UPCRMonsterDataAsset;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogSpearActor, Log, All);

DECLARE_MULTICAST_DELEGATE(FDetachedSpearSignature);
DECLARE_MULTICAST_DELEGATE(FDestroyedSpearSigniture);

UCLASS()
class PROJECTCARDRETURN_API APCRSpearActor : public AActor//, public IPCREricaCardInteractable
{
	GENERATED_BODY()
	
public:	
	APCRSpearActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

public:
	void DetachAndDelayedDestroy();
	void Throw(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction);
	FORCEINLINE bool IsThrown() const { return Thrown; }
	
	FDetachedSpearSignature OnDetachedSpear;
	FDestroyedSpearSigniture OnDestroyedSpear;
	
private:
	void DelayedDestroy();
	void DestroyTimerCallback();

	UFUNCTION()
	void HandleSpearHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//UFUNCTION()
	//void HandleBlockedByCard(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	//virtual void BindOnCardReturnBegin(APCREricaCardProjectile* AttachedCard) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Move")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY()
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRParameterDataAsset> ParameterDataAsset;

	uint32 Thrown:1;
};
