// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Animation/AnimInstance.h"
#include "PCRSerinDollHandAnimInstance.generated.h"

DECLARE_DELEGATE(FIdleSignature);
DECLARE_MULTICAST_DELEGATE(FRockAttackSignature);
DECLARE_MULTICAST_DELEGATE(FPaperAttackSignature);
DECLARE_MULTICAST_DELEGATE(FScissorsAttackSignature);

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

public:
	UPCRSerinDollHandAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;

public:
	void PlayRockAttack();
	void PlayPaperAttack(bool bIsFar = false);
	void PlayScissorsAttack();

public: // 델리게이트
	FIdleSignature OnToIdle;
	
	FRockAttackSignature OnRockAttackEnded;
	FRockAttackSignature OnRockAttackHit;

	FPaperAttackSignature OnPaperAttackSweepStart;
	FPaperAttackSignature OnPaperAttackSweepEnd;

	FScissorsAttackSignature OnScissorsAttackEffectStart;
	FScissorsAttackSignature OnScissorsAttackHitStart;
	FScissorsAttackSignature OnScissorsAttackHitEnd;

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
	void AnimNotify_ToIdle();

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
