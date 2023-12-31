// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAnimInstance.h"
#include "PCRMeleeSoldierAnimInstance.generated.h"

class APCRMeleeSoldierCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogMeleeSoldierAnimInstance, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRMeleeSoldierAnimInstance : public UPCRMonsterBaseAnimInstance
{
	GENERATED_BODY()

public:
	UPCRMeleeSoldierAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void Hit();

	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	TObjectPtr<APCRMeleeSoldierCharacter> CachedMeleeSoldier;
	
	UPROPERTY(BlueprintReadOnly, Category = "AnimationState")
	uint32 bHasShield:1;

private:
	UFUNCTION()
	void AnimNotify_Hit();
	
	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void ShieldAttack();
	void SpearAttack();
};
