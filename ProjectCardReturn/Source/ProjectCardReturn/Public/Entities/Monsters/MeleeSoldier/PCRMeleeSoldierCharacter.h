// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "PCRMeleeSoldierCharacter.generated.h"


class APCRShieldActor;

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
	
	UFUNCTION(BlueprintCallable)
	bool GetOwnShield() const { return bOwnShield; }
	
	void SpawnAndAttachShield();
	void HandleDetachedShield();

protected:
	virtual void HandleDead() override;
	
private:
	UPROPERTY()
	TObjectPtr<APCRShieldActor> Shield;

	bool bCanAttack;
	bool bOwnShield;
};
