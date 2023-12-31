// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAnimInstance.h"
#include "PCRRangedSoldierAnimInstance.generated.h"

class APCRRangedSoldierCharacter;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogRangedSoldierAnimInstance, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRRangedSoldierAnimInstance : public UPCRMonsterBaseAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void Throw();

	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	TObjectPtr<APCRRangedSoldierCharacter> CachedRangedSoldier;

private:
	UFUNCTION()
	void AnimNotify_Throw();

	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
