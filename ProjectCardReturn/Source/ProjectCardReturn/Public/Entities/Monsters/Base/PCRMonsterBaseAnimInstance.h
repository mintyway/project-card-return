// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PCRMonsterBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRMonsterBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPCRMonsterBaseAnimInstance();

private:
	UPROPERTY(BlueprintReadOnly, Category = "AnimationState")
	
};
