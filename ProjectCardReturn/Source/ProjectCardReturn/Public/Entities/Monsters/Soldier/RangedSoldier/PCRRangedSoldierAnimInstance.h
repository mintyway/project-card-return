// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAnimInstance.h"
#include "PCRRangedSoldierAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRRangedSoldierAnimInstance : public UPCRMonsterBaseAnimInstance
{
	GENERATED_BODY()

public:
	UPCRRangedSoldierAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void Throw();
};
