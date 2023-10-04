// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "PCRMeleeSoldierCharacter.generated.h"


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
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void HandleDead() override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Dummy")
	TObjectPtr<UStaticMeshComponent> DummyMeshComponent;

	UPROPERTY()
	TObjectPtr<AActor> Shield;
	
	bool bCanAttack;
};
