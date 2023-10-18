// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "PCRSoldierBaseCharacter.generated.h"


class UPCRMeleeSoldierAnimInstance;
class APCRShieldActor;
class APCRSpearActor;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogMeleeSoldierCharacter, Log, All);

UENUM()
enum class EAttackType : uint8
{
	Melee,
	Ranged
};

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRSoldierBaseCharacter : public APCRMonsterBaseCharacter
{
	GENERATED_BODY()

public:
	APCRSoldierBaseCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	FORCEINLINE EAttackType GetAttackType() const { return AttackType; }
	FORCEINLINE bool GetHasShield() const { return bHasShield; }
	FORCEINLINE bool GetHasSpear() const { return bHasSpear; }

	void SpawnAndAttachShield();
	void HandleDetachedShield();
	void SpawnAndAttachSpear();
	void HandleDetachedSpear();
	
	virtual void Attack() override;
	
protected:
	virtual void HandleDead() override;

	UPROPERTY()
	TObjectPtr<APCRShieldActor> Shield;

	UPROPERTY()
	TObjectPtr<APCRSpearActor> Spear;
	
	EAttackType AttackType;
	uint32 bHasShield:1;
	uint32 bHasSpear:1;
	
private:
	UPROPERTY()
	TObjectPtr<UPCRMeleeSoldierAnimInstance> MeleeSoldierAnimInstance;

	uint32 bCanAttack:1;
};
