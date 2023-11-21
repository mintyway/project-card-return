// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Animation/AnimInstance.h"
#include "PCRMonsterBaseAnimInstance.generated.h"

class UPCRMonsterDataAsset;
class APCRMonsterBaseCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRMonsterBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPCRMonsterBaseAnimInstance();

	FORCEINLINE bool GetCanAttack() const { return bCanAttack; }

	virtual void Attack();

protected:
	void HandleOwnerDead(APCRMonsterBaseCharacter* DeadMonster);
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	TObjectPtr<APCRMonsterBaseCharacter> CachedMonsterBaseCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	TObjectPtr<UCharacterMovementComponent> CachedCharacterMovement;
	
	UPROPERTY(BlueprintReadOnly, Category = "AnimationState")
	uint32 bShouldMove:1;
	
	UPROPERTY(BlueprintReadOnly, Category = "AnimationState")
	uint32 bCanAttack:1;
	
	UPROPERTY(BlueprintReadOnly, Category = "AnimationState")
	uint32 bDead:1;

	UPROPERTY(BlueprintReadOnly, Category = "AnimationState")
	FVector Velocity;
	
	UPROPERTY()
	TObjectPtr<const UPCRMonsterDataAsset> MonsterDataAsset;

private:
	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
