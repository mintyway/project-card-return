// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "PCRMeleeSoldierCharacter.generated.h"


class UPCRMeleeSoldierAnimInstance;
class APCRShieldActor;
class APCRSpearActor;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogMeleeSoldierCharacter, Log, All);

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRMeleeSoldierCharacter : public APCRMonsterBaseCharacter
{
	GENERATED_BODY()

public:
	APCRMeleeSoldierCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void Attack() override;
	
	FORCEINLINE bool GetHasShield() const { return bHasShield; }
	
	void SpawnAndAttachShield();
	//
	void SpawnAndAttachSpear();
	void HandleDetachedShield();
	void HandleDetachedSpear();

protected:
	virtual void HandleDead() override;
	
private:
	UPROPERTY()
	TObjectPtr<APCRShieldActor> Shield;

	//
	UPROPERTY()
	TObjectPtr<APCRSpearActor> Spear;
	
	UPROPERTY()
	TObjectPtr<UPCRMeleeSoldierAnimInstance> MeleeSoldierAnimInstance;

	uint32 bCanAttack:1;
	uint32 bHasShield:1;
	// 
	uint32 bHasSpear:1;
};
