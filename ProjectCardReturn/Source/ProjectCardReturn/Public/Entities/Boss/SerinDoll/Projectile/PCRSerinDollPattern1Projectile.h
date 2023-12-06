// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PCREricaCardInteractable.h"
#include "PCRSerinDollPattern1Projectile.generated.h"

class UNiagaraComponent;
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

private:
	DECLARE_DELEGATE(FAttachedCardSignature);
	
public:	
	APCRSerinDollPattern1Projectile();

protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

public:
	void Shoot(AActor* NewOwner, const FVector& InStartLocation, const FVector& InTargetLocation);
	void Release();
	void Pattern1ExplosionTimerStart();

public:
	void BindOnReturnCardBegin(APCREricaCardProjectile* AttachedCard) override;

public: // 델리게이트
	FAttachedCardSignature OnDetachedCard;

private: // 내부 함수
	void EnableProjectile();
	void DisableProjectile();
	
	void EnableCollisionDetection();
	void DisableCollisionDetection();

	void HandleShooting();
	
	bool IsAtMaxRange();
	void HandleStop();

	void HandlePattern1DetachedCard(APCREricaCardProjectile* AttachedCard);
	void HandlePattern1Explosion();
	void HandlePattern1ExplosionTimerSound();

	UFUNCTION()
	void HandleBossOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> Pattern1ThrowEffectComponent;

	UPROPERTY()
	TArray<TObjectPtr<UNiagaraComponent>> Pattern1SparkEffectComponents;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> Pattern1BombTimerEffectComponent;

private: // 데이터
	ESerinDollProjectileState State;
	FTimerHandle TimerSoundHandle;
	FTimerHandle HandlePattern1ExplosionTimerHandle;
	FVector LaunchLocation;
	FVector TargetLocation;
	float ProjectileSpeed;
	float ProjectileReturnSpeed;
	uint32 bOnceDetached:1;
	uint32 bCanTakeDamageToErica:1;
};
