// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollBaseCharacter.h"
#include "PCRSerinDollHandBaseCharacter.generated.h"

class APCRSerinDollCharacter;
class APCRSerinDollHandBaseCharacter;
class APCRSerinDollLeftHandCharacter;
class APCRSerinDollRightHandCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogSerinHandBaseCharacter, Log, All);

DECLARE_DELEGATE(FHandStateSignature);
DECLARE_DELEGATE(FMoveEndSignature);


UENUM()
enum class ESerinState
{
	Idle,
	Move,
	Chase,
	BasicChase,
	Rock,
	Paper,
	Scissors,
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // 동작
	void SetTarget(AActor* TargetActor);

	void Move(const FVector& InLocation, bool bUseReset = true);
	void BasicChase(bool bUseReset = true);
	void Chase(bool bUsePredictive = false, bool bUseReset = true);
	void RockAttack(bool bUseReset = true);
	void PaperAttack(bool bUseReset = true);
	void Scissors(bool bUseReset = true);

public:
	FHandStateSignature OnChaseEnd;
	FMoveEndSignature OnMoveEnd;

protected: // 내부 함수
	void StateReset();
	void HandleMove(float DeltaTime);
	virtual void HandleBasicChase(float DeltaTime);
	void HandleChase(float DeltaTime);
	void RockCallback();
	void HandleRock(float DeltaTime);

protected: // 캐시
	UPROPERTY()
	TObjectPtr<APCRSerinDollCharacter> CachedSerinDollCharacter;

	UPROPERTY()
	TObjectPtr<AActor> CachedTarget;

protected: // 상태
	ESerinState CurrentSerinState;

	uint32 bUsePredictiveChase : 1;

protected: // 데이터
	FVector MoveLocation;
	FVector ChaseLocation;
};
