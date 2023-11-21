// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAnimInstance.h"
#include "PCRRabbitAnimInstance.generated.h"

class APCRRabbitCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRRabbitAnimInstance, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRRabbitAnimInstance : public UPCRMonsterBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UPCRRabbitAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	void Hit();

	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	TObjectPtr<APCRRabbitCharacter> CachedRabbit;
	
	UPROPERTY(BlueprintReadOnly, Category = "AnimationState")
	uint32 bJump:1;

private:
	UFUNCTION()
	void AnimNotify_Hit();
};
