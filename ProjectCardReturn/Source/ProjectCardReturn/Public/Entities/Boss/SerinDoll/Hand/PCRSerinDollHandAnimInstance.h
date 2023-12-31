// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Animation/AnimInstance.h"
#include "PCRSerinDollHandAnimInstance.generated.h"


class APCRSerinDollHandCharacter;
class UPCRSerinDollPrimaryDataAsset;
class UPCRParameterDataAsset;

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRSerinDollHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private: // 타입
	DECLARE_DELEGATE(FIdleSignature);
	DECLARE_MULTICAST_DELEGATE(FRockAttackSignature);
	DECLARE_MULTICAST_DELEGATE(FPaperAttackSignature);
	DECLARE_MULTICAST_DELEGATE(FScissorsAttackSignature);
	DECLARE_MULTICAST_DELEGATE(FPattern1Signature);
	DECLARE_MULTICAST_DELEGATE(FPattern2Signature);

public:
	UPCRSerinDollHandAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;

public: // 동작
	void PlayStart();
	void PlayRockAttack();
	void PlayPaperAttack(bool bIsFarAttack = false);
	void PlayScissorsAttack();
	
	void PlayPattern1(bool IsLeftHand);
	void EndPattern1();

	void PlayPattern2();
	void HitPattern2();
	void EndPattern2();

public: // 델리게이트
	FIdleSignature OnToIdle;
	
	FRockAttackSignature OnRockAttackEnded;
	FRockAttackSignature OnRockAttackHit;

	FPaperAttackSignature OnPaperAttackSweepStart;
	FPaperAttackSignature OnPaperAttackSweepEnd;

	FScissorsAttackSignature OnScissorsAttackEffectStart;
	FScissorsAttackSignature OnScissorsAttackHitStart;
	FScissorsAttackSignature OnScissorsAttackHitEnd;

	FPattern1Signature OnPattern1Shoot;
	FPattern1Signature OnPattern1LastShoot;
	FPattern1Signature OnPattern1Ended;

	FPattern2Signature OnPattern2CollisionActivate;

private: // 애님 노티파이
	UFUNCTION()
	void AnimNotify_RockAttackChaseEnd();

	UFUNCTION()
	void AnimNotify_RockAttackHit();

	UFUNCTION()
	void AnimNotify_PaperAttackSweepStart();

	UFUNCTION()
	void AnimNotify_PaperAttackSweepEnd();

	UFUNCTION()
	void AnimNotify_ScissorsAttackHitStart();
	
	UFUNCTION()
	void AnimNotify_ScissorsAttackHitEnd();
	
	UFUNCTION()
	void AnimNotify_ScissorsAttackCountCheck();

	UFUNCTION()
	void AnimNotify_ScissorsAttackEffectStart();

	UFUNCTION()
	void AnimNotify_Pattern1Shoot();

	UFUNCTION()
	void AnimNotify_ToIdle();

	UFUNCTION()
	void AnimNotify_Pattern1LastShoot();

	UFUNCTION()
	void AnimNotify_Pattern2CollisionActivate();

	void HandlePattern1Ended(UAnimMontage* AnimMontage, bool bArg);

private: // 레퍼런스
	UPROPERTY()
	TObjectPtr<UPCRSerinDollPrimaryDataAsset> SerinDollDataAsset;

	UPROPERTY()
	TObjectPtr<UPCRParameterDataAsset> ParameterDataAsset;

	UPROPERTY()
	TObjectPtr<APCRSerinDollHandCharacter> CachedSerinDollHand;

private: // 데이터
	int32 CurrentScissorsAttackCount;
};
