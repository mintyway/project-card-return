// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollBaseCharacter.h"
#include "PCRSerinDollHandBaseCharacter.generated.h"

class UPCRSerinDollHandBaseAnimInstance;
class APCRSerinDollHeadCharacter;
class APCRSerinDollHandBaseCharacter;
class APCRSerinDollLeftHandCharacter;
class APCRSerinDollRightHandCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogSerinHandBaseCharacter, Log, All);

DECLARE_DELEGATE(FChaseEndSignature);
DECLARE_DELEGATE(FMoveEndSignature);


UENUM()
enum class ESerinState
{
	Idle,
	Move,
	BasicChase,
	PaperChase,
	RockChase,
	Rock,
	Paper,
	Scissors,
	Invalid
};

UENUM()
enum class EScissorsState
{
	Stay,
	Attack,
	Invalid
};

UCLASS(Abstract)
class PROJECTCARDRETURN_API APCRSerinDollHandBaseCharacter : public APCRSerinDollBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinDollHandBaseCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // 동작
	void SetSerinDollCharacter(APCRSerinDollHeadCharacter* InSerinDollCharacter);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void SetTarget(AActor* TargetActor);

	void Move(const FVector& InLocation, bool bUseReset = true);
	void BasicChase(bool bUseReset = true);
	virtual void PaperChase(bool bUseReset = true);
	void RockChase(bool bUsePredictive = false, bool bUseReset = true);
	void RockAttack(bool bUseReset = true);
	void PaperAttack(bool bUseReset = true);
	void ScissorsAttack(bool bUseReset = true);

public:
	FMoveEndSignature OnMoveEnd;
	FChaseEndSignature OnPaperChaseEnd;
	FChaseEndSignature OnHighChaseEnd;

protected: // 내부 함수
	void StateReset();
	void HandleMove(float DeltaTime);
	virtual void HandleBasicChase(float DeltaTime);
	virtual void HandlePaperChase(float DeltaTime);
	void HandleRockChase(float DeltaTime);
	void RockCallback();
	void HandleRock(float DeltaTime);
	virtual void PaperCallback();
	void HandlePaper(float DeltaTime);
	void ScissorsCallback();
	void HandleScissors(float DeltaTime);
	void HandleEndedScissorsAttack();

protected: // 캐시
	UPROPERTY()
	TObjectPtr<UPCRSerinDollHandBaseAnimInstance> CachedSerinDollHandBaseAnimInstance;

	UPROPERTY()
	TObjectPtr<APCRSerinDollHeadCharacter> CachedSerinDollCharacter;

	UPROPERTY()
	TObjectPtr<AActor> CachedTarget;

protected: // 상태
	ESerinState CurrentSerinState;
	EScissorsState CurrentScissorsState;

	uint32 bUsePredictiveChase:1;
	uint32 bPaperStartFlag:1;

protected: // 데이터
	FVector MoveLocation;
	FVector ChaseLocation;
	FRotator PaperRotation;

	float CoolDown;
	float CoolDownElapsedTime;

	int32 MaxScissorsAttackCount;
	int32 CurrentScissorsAttackCount;
};
