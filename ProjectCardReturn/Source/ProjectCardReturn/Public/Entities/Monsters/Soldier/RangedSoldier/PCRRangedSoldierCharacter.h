// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Monsters/Soldier/Base/PCRSoldierBaseCharacter.h"
#include "PCRRangedSoldierCharacter.generated.h"

class APCRSpearActor;
class UPCRRangedSoldierAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRRangedSoldierCharacter : public APCRSoldierBaseCharacter
{
	GENERATED_BODY()

public:
	APCRRangedSoldierCharacter();

protected:
	virtual void PostInitializeComponents() override;

public:
	FORCEINLINE bool GetHasSpear() const { return Spear != nullptr; }

	void SpawnAndAttachSpear();
	void HandleDetachedSpear();
	void HandleDestroyedSpear();

	virtual void Attack() override;

protected:
	virtual void HandleDead() override;

	UPROPERTY()
	TObjectPtr<APCRSpearActor> Spear;

private:
	UPROPERTY()
	TObjectPtr<UPCRRangedSoldierAnimInstance> RangedSoldierAnimInstance;

	UPROPERTY()
	TObjectPtr<ACharacter> CachedPlayerCharacter;
};
