// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Animation/AnimInstance.h"
#include "PCRSerinDollHeadAnimInstance.generated.h"

class UPCRParameterDataAsset;
class UPCRSerinDollPrimaryDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRSerinDollHeadAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPCRSerinDollHeadAnimInstance();

public: // 동작
	void PlayStart();
	void PlayHit();
	void PlayPattern1();
	void EndPattern1();
	
	void PlayPattern2();
	void EndPattern2();

private:
	UPROPERTY()
	TObjectPtr<UPCRSerinDollPrimaryDataAsset> SerinDollDataAsset;

	UPROPERTY()
	TObjectPtr<UPCRParameterDataAsset> ParameterDataAsset;
};
