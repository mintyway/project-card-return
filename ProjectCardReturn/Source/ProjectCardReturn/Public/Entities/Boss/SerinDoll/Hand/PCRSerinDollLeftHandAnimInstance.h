// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Boss/SerinDoll/Hand/Base/PCRSerinDollHandBaseAnimInstance.h"
#include "PCRSerinDollLeftHandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRSerinDollLeftHandAnimInstance : public UPCRSerinDollHandBaseAnimInstance
{
	GENERATED_BODY()

public:
	UPCRSerinDollLeftHandAnimInstance();
	
public:
	virtual void PlayScissorsAttack() override;

protected:
	virtual void ScissorsAttackCallback(UAnimMontage* AnimMontage, bool bArg);

private:
};
