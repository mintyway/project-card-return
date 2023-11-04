// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollBaseCharacter.h"
#include "PCRSerinDollHandCharacter.generated.h"

class UPCRSerinDollHandAnimInstance;
DECLARE_LOG_CATEGORY_EXTERN(PCRLogSerinHandCharacter, Log, All);

DECLARE_DELEGATE(FMoveCallbackSignature);
DECLARE_DELEGATE(FChaseCallbackSignature);

class APCRSerinDollHeadCharacter;
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
	void Init(APCRSerinDollHeadCharacter* NewSerinDollHead, const FVector& InIdleOffsetFromTarget);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Idle(AActor* NewTarget);

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
		float IdleChaseSpeed;
	};

private: // 업데이트 함수
	void UpdateIdle(float DeltaTime);

private: // 내부 함수
	
private: // 내부 델리게이트
	FMoveCallbackSignature OnMoveEndedCallback;
	FChaseCallbackSignature OnChaseEndedCallback;

private: // 레퍼런스
	UPROPERTY()
	TObjectPtr<APCRSerinDollHeadCharacter> CachedSerinDollHead;

	UPROPERTY()
	TObjectPtr<UPCRSerinDollHandAnimInstance> CachedSerinDollHandAnimInstance;

private: // 데이터
	EState CurrentState;
	FVector IdleOffsetFromTarget;
	FIdleData IdleData;
};
