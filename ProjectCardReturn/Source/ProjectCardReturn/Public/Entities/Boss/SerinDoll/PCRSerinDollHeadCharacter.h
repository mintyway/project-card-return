// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollBaseCharacter.h"
#include "PCRSerinDollHeadCharacter.generated.h"

class APCRLiftActor;
class APCREricaCharacter;
class APCRSerinDollHandCharacter;
class APCRSerinDollHandBaseCharacter;
class APCRSerinDollLeftHandCharacter;
class APCRSerinDollRightHandCharacter;

DECLARE_MULTICAST_DELEGATE_TwoParams(FChangeHPSignature, float, float);

UCLASS()
class PROJECTCARDRETURN_API APCRSerinDollHeadCharacter : public APCRSerinDollBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSerinDollHeadCharacter();

	friend class APCRSerinDollHandCharacter;
	friend class APCRSerinDollHandBaseCharacter;
	friend class APCRSerinDollLeftHandCharacter;
	friend class APCRSerinDollRightHandCharacter;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public: // Getter, Setter
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }

	// 델리게이트
	FChangeHPSignature OnChangeHP;

private: // 내부 함수
	void SpawnHands();
	void ChangeHP(float Amount);
	void HandleChangeHP();
	void HandleDead();
	void DelayedDestroy();

private: // 내부 Getter
	float GetLiftHeight();
	float GetHandWorldHeight();

private: // 캐시
	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedErica;

	UPROPERTY()
	TObjectPtr<APCRLiftActor> CachedLift;

private: // 핸드
	UPROPERTY()
	TObjectPtr<APCRSerinDollHandCharacter> LeftHand;

	UPROPERTY()
	TObjectPtr<APCRSerinDollRightHandCharacter> RightHand;

	static const float ContactDistance;
	static const float FloatingHandHeight;
	static const float BasicChaseYDistance;

private: // 데이터
	float IdleWidthOffsetFromErica;
	float IdleHeightOffsetFromErica;
	float MaxHP;
	float CurrentHP;

private:
	TArray<FTimerHandle> TimerHandles;
	uint32 bIsAlive:1;
};
