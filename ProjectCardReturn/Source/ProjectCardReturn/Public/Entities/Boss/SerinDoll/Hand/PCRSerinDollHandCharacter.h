// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollBaseCharacter.h"
#include "PCRSerinDollHandCharacter.generated.h"

class UNiagaraComponent;
class UPCRSerinDollHandAnimInstance;
class APCRSerinDollHeadCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogSerinHandCharacter, Log, All);

DECLARE_DELEGATE(FMoveCallbackSignature);
DECLARE_DELEGATE(FChaseCallbackSignature);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRSerinDollHandCharacter : public APCRSerinDollBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinDollHandCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public: // 동작
	void Init(APCRSerinDollHeadCharacter* NewSerinDollHead, const FVector& InSideVector);
	void Idle(AActor* NewTarget);
	void RockAttack(AActor* NewTarget);
	void PaperAttack(bool bIsFar);
	void ScissorsAttack(AActor* NewTarget);

public: // Getter, Setter
	FORCEINLINE bool IsIdle() { return CurrentState == EState::Idle; }
	FORCEINLINE int32 GetMaxScissorsAttackCount() { return ScissorsAttackData.MaxAttackCount; }
	FORCEINLINE int32 GetCurrentScissorsAttackCount() { return ScissorsAttackData.CurrentAttackCount; }

private: // 타입
	enum class EState
	{
		Idle,
		Move,
		Chase,
		RockAttack,
		PaperAttack,
		ScissorsAttack
	};

	struct FIdleData
	{
		const AActor* Target;
		float ChaseLocationExponentialSpeed;
		float ChaseRotationExponentialSpeed;
	};

	struct FRockAttackData
	{
		const AActor* Target;
		float ChaseLocationSpeed;
		float ChaseRotationExponentialSpeed;
		float ChaseHeight;
		uint32 bIsChasing:1;
	};

	struct FPaperAttackData
	{
		const AActor* Lift;
		FVector Offset;
		FVector MoveLocation;
		float MoveLocationSpeed;
		float MoveRotationExponentialSpeed;
		uint32 bIsFar:1;
		uint32 bIsMoving:1;
	};

	struct FScissorsAttackData
	{
		const AActor* Target;
		float ChaseLocationExponentialSpeed;
		float ChaseRotationExponentialSpeed;
		float ChaseDistance;
		int32 MaxAttackCount;
		int32 CurrentAttackCount;
		uint32 bIsChasing:1;
	};

private: // 업데이트 함수
	void UpdateIdle(float DeltaSeconds);
	void UpdateRockAttackChase(float DeltaSeconds);
	void UpdatePaperAttackMove(float DeltaSeconds);
	void UpdateScissorsAttackChase(float DeltaSeconds);

private: // 내부 함수
	void PlayRockAttackEffect();
	void PlayPaperAttackEffect();
	void StopPaperAttackEffect();
	void PlayScissorsAttackEffect();
	
public: // 외부 콜백
	void HandleToIdle();
	void HandleRockAttackChaseEnded();

private: // 내부 델리게이트
	FMoveCallbackSignature OnMoveEndedCallback;
	FChaseCallbackSignature OnChaseEndedCallback;

private: // 레퍼런스
	UPROPERTY()
	TObjectPtr<APCRSerinDollHeadCharacter> CachedSerinDollHead;

	UPROPERTY()
	TObjectPtr<UPCRSerinDollHandAnimInstance> CachedSerinDollHandAnimInstance;

private: // 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> RockAttackNiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> PaperAttackNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraComponent> ScissorsAttackNiagaraComponent;

private: // 데이터
	EState CurrentState;
	FIdleData IdleData;
	FRockAttackData RockAttackData;
	FPaperAttackData PaperAttackData;
	FScissorsAttackData ScissorsAttackData;

	FVector SideVector;
	float IdleSideOffset;
	float IdleUpOffset;
	FVector IdleOffsetFromTarget;
	float DefaultSpeed;
};
