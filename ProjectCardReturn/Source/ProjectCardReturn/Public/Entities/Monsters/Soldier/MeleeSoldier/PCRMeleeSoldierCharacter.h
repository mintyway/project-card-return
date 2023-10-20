// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Monsters/Soldier/Base/PCRSoldierBaseCharacter.h"
#include "PCRMeleeSoldierCharacter.generated.h"

class APCRShieldActor;
class UPCRMeleeSoldierAnimInstance;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRMeleeSoldierCharacter : public APCRSoldierBaseCharacter
{
	GENERATED_BODY()

public:
	APCRMeleeSoldierCharacter();

protected:
	virtual void PostInitializeComponents() override;

public:
	FORCEINLINE bool GetHasShield() const { return Shield != nullptr; }

	void SpawnAndAttachShield();
	void HandleDetachedShield();

	virtual void Attack() override;

protected:
	virtual void HandleDead() override;

	UPROPERTY()
	TObjectPtr<APCRShieldActor> Shield;

private:
	UPROPERTY()
	TObjectPtr<UPCRMeleeSoldierAnimInstance> MeleeSoldierAnimInstance;
};