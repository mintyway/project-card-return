// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Projectiles/Base/PCRBaseProjectile.h"
#include "PCREricaCardProjectile.generated.h"

class UNiagaraComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FReturnCardBeginSignature, APCREricaCardProjectile*);

DECLARE_LOG_CATEGORY_EXTERN(PCRLogEricaCardProjectile, Log, All);

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

private: // 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "FX")
	TObjectPtr<UNiagaraComponent> CardRibbonFXComponent;

	UPROPERTY(VisibleAnywhere, Category = "FX")
	TObjectPtr<UNiagaraComponent> CardFloatingFXComponent;

private: // 데이터
	float ForwardDamage;
	float BackwardDamage;
	float CardReturnSpeed;
	float CardRange;
	// 카드가 플레이어로부터 얼마나 가까워지면 회수될지 거리를 나타냅니다.
	float CardReleaseRange;

	ECardState CurrentCardState;

	UPROPERTY()
	TArray<TObjectPtr<ACharacter>> AttackedCharacter;
};
