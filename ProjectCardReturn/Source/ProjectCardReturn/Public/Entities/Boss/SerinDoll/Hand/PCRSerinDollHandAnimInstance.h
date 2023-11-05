// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Animation/AnimInstance.h"
#include "PCRSerinDollHandAnimInstance.generated.h"

DECLARE_DELEGATE(FScissorsSignature);

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
	void PlayRock();
	void PlayPaper();
	void PlayScissorsAttack(APCRSerinDollHandCharacter* InSerinDollHand);

public:
	FScissorsSignature OnScissorsAttackEnded;

private: // 애님 노티파이
	UFUNCTION()
	void AnimNotify_ScissorsAttackCountCheck();

	UFUNCTION()
	void AnimNotify_ToIdle();

private:  // 레퍼런스
	UPROPERTY()
	TObjectPtr<UPCRSerinDollPrimaryDataAsset> SerinDollDataAsset;

	UPROPERTY()
	TObjectPtr<UPCRParameterDataAsset> ParameterDataAsset;

	UPROPERTY()
	TObjectPtr<APCRSerinDollHandCharacter> CachedSerinDollHand;

private: // 데이터
	int32 CurrentScissorsAttackCount;
};
