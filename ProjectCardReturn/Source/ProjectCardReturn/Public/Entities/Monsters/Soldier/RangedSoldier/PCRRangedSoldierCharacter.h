// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Soldier/Base/PCRSoldierBaseCharacter.h"
#include "PCRRangedSoldierCharacter.generated.h"

class UNiagaraComponent;
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
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void Attack() override;

public:
	void Throw();
	FVector SpearDirection();

private:
	UPROPERTY()
	TObjectPtr<ACharacter> CachedPlayerCharacter;

	UPROPERTY()
	TObjectPtr<UPCRRangedSoldierAnimInstance> AnimInstance;
	
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> WarnEffectNiagaraComponent;
};
