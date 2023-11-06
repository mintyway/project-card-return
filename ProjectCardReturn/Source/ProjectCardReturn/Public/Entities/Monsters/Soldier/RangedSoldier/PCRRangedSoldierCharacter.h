// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
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
	virtual void Attack() override;
	
	void Throw();

private:
	UPROPERTY()
	TObjectPtr<UPCRRangedSoldierAnimInstance> RangedSoldierAnimInstance;

	UPROPERTY()
	TObjectPtr<ACharacter> CachedPlayerCharacter;
};
