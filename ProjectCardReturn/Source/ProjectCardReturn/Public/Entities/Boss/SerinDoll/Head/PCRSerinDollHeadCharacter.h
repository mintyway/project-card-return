// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollBaseCharacter.h"
#include "PCRSerinDollHeadCharacter.generated.h"

class UPCRSerinDollHeadAnimInstance;
class APCRLiftActor;
class APCREricaCharacter;
class APCRSerinDollHandCharacter;
class APCRSerinDollHandBaseCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogSerinDollHeadCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE_TwoParams(FChangeHPSignature, float, float);
DECLARE_MULTICAST_DELEGATE(FHPStateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPattern2Signature);

UCLASS()
class PROJECTCARDRETURN_API APCRSerinDollHeadCharacter : public APCRSerinDollBaseCharacter
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FPattern1InternalSignature);
	DECLARE_MULTICAST_DELEGATE(FPattern2InternalSignature);

public:
	APCRSerinDollHeadCharacter();

	friend class APCRSerinDollHandCharacter;
	friend class APCRSerinDollHandBaseCharacter;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public: // 동작
	UFUNCTION(BlueprintCallable)
	void SetMeshVisibility(bool bIsVisible);
	void DestroySerinDoll();
	
	void LeftRockAttack();
	void RightRockAttack();

	void LeftPaperAttack(bool bIsFarAttack);
	void RightPaperAttack(bool bIsFarAttack);

	void LeftScissorsAttack();
	void RightScissorsAttack();

	void Pattern1();
	void Pattern2();

public: // Getter, Setter
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }
	FORCEINLINE APCRLiftActor* GetCachedLift() const { return CachedLift; }
	FORCEINLINE APCREricaCharacter* GetCachedErica() const { return CachedErica; }
	FORCEINLINE APCRSerinDollHandCharacter* GetLeftHand() const { return LeftHand; }
	FORCEINLINE APCRSerinDollHandCharacter* GetRightHand() const { return RightHand; }

	// 델리게이트
	FChangeHPSignature OnChangeHP;
	FHPStateSignature OnHP50PercentLess;
	
	FPattern1InternalSignature OnPattern1StartInternal;
	FPattern1InternalSignature OnPattern1EndedInternal;
	FPattern1InternalSignature OnPattern1SucceedInternal;

	FPattern2InternalSignature OnPattern2SucceedInternal;
	
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FPattern2Signature OnPattern2Succeed;

private: // 타입
	enum class EState
	{
		Basic,
		Pattern1,
		Pattern2,
		End
	};

	struct FPattern1Data
	{
		int32 DetachCount;
		int32 DetachAttackCount;
		uint32 bMoveStarted:1;
		uint32 bIsStarted:1;
		uint32 bIsLeftHandReady:1;
		uint32 bIsRightHandReady:1;
	};

	struct FPattern2Data
	{
		int32 SumPulledCount;
	};

private: // 내부 함수
	void SpawnHands();
	void LeftHandSpawn();
	void RightHandSpawn();
	void ChangeHP(float Amount);
	void HandleChangeHP();
	void HandleDead();

	bool IsAttacking(APCRSerinDollHandCharacter* InSerinDollHand);

	void Pattern1DetachCountCheck();
	void HandleReadyPattern1(bool IsLeftHand);
	void HandlePattern1Succeed();
	void HandlePattern1Ended();
	void HandlePattern1LastShoot();
	void HandleRestartPattern1();

	void HandlePattern2CardPull();
	void HandlePattern2Succeed();

	// 테스트용
	void AttackTest();

private: // 내부 Getter
	float GetLiftHeight();
	float GetHandWorldHeight();

private: // 레퍼런스
	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedErica;

	UPROPERTY()
	TObjectPtr<APCRLiftActor> CachedLift;

	UPROPERTY()
	TObjectPtr<UPCRSerinDollHeadAnimInstance> CachedSerinDollHeadAnimInstance;

private: // 핸드
	UPROPERTY()
	TObjectPtr<APCRSerinDollHandCharacter> LeftHand;

	UPROPERTY()
	TObjectPtr<APCRSerinDollHandCharacter> RightHand;

	static const float ContactDistance;
	static const float FloatingHandHeight;
	static const float BasicChaseYDistance;

private: // 데이터
	float MaxHP;
	float CurrentHP;

private:
	EState State;
	FPattern1Data Pattern1Data;
	FPattern2Data Pattern2Data;
	TArray<FTimerHandle> TimerHandles;
	uint32 bIsAlive:1;
	uint32 bIsInvincible;
	uint32 bIsHP50PercentLess:1;
	uint32 bIsHP0PercentLess:1;
};
