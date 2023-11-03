// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Animation/AnimInstance.h"
#include "PCRSerinDollHandBaseAnimInstance.generated.h"

class UPCRParameterDataAsset;
class UPCRSerinDollPrimaryDataAsset;

DECLARE_DELEGATE(FScissorsAttackSignature);
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRSerinDollHandBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPCRSerinDollHandBaseAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

public:
	virtual void PlayScissorsAttack() PURE_VIRTUAL();

public:
	FScissorsAttackSignature OnScissorsAttackEnded;

protected:
	virtual void ScissorsAttackCallback(UAnimMontage* AnimMontage, bool bArg) PURE_VIRTUAL();

protected:
	UPROPERTY()
	TObjectPtr<UPCRSerinDollPrimaryDataAsset> SerinDollDataAsset;

	UPROPERTY()
	TObjectPtr<UPCRParameterDataAsset> ParameterDataAsset;
};
