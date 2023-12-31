// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Projectiles/Base/PCRBaseProjectile.h"
#include "PCREricaCardProjectile.generated.h"

class UNiagaraComponent;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogEricaCardProjectile, Log, All);

DECLARE_MULTICAST_DELEGATE_OneParam(FReturnCardBeginSignature, APCREricaCardProjectile*);

UENUM()
enum class ECardState : uint8
{
	Flying = 1,
	Stop,
	Returning,
	Invalid = 0
};

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCREricaCardProjectile : public APCRBaseProjectile
{
	GENERATED_BODY()

public:
	APCREricaCardProjectile();

protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

public: // 동작
	virtual void LaunchProjectile(AActor* NewOwner, const FVector& StartLocation, const FVector& Direction) override;
	virtual void ReleaseToProjectilePool() override;
	
	void ReturnCard();

public: // Getter, Setter
	void SetDamage(float InForwardDamage, float InBackwardDamage);
	void SetRange(float NewRange) { CardRange = NewRange; }
	FORCEINLINE ECardState GetCurrentCardState() const { return CurrentCardState; }

public: // 델리게이트
	FReturnCardBeginSignature OnReturnCardBegin;

protected:
	virtual void EnableCollisionDetection() override;
	
private: // 내부 함수
	UFUNCTION()
	void HandleBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void HandleBlocking(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	
	void PauseCard();

	void HandleCardReturn(float DeltaSeconds);
	void CheckCardRangeAndStop(float DeltaSeconds);
	void HandleCardMaxRange();

	void InitCollisionObjectQueryParams();

private: // 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> CardShotRibbonFXComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> CardRecallRibbonFXComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> CardFloatingFXComponent;

private: // 데이터
	float ForwardDamage;
	float BackwardDamage;
	float CardReturnSpeed;
	float CardRange;
	float KnockBackPower;
	FVector LastTickLocation;
	FVector LastTickForwardDirection;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> AttackedActors;
	
	ECardState CurrentCardState;
	FCollisionObjectQueryParams CollisionObjectQueryParams;
};
