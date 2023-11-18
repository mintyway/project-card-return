// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	UPROPERTY()
	TObjectPtr<UPCRSerinDollPrimaryDataAsset> SerinDollDataAsset;

	UPROPERTY()
	TObjectPtr<UPCRParameterDataAsset> ParameterDataAsset;
};
