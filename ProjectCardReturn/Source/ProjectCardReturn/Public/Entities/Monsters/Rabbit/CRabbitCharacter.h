// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/CMonsterBaseCharacter.h"
#include "CRabbitCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API ACRabbitCharacter : public ACMonsterBaseCharacter
{
	GENERATED_BODY()

public:
	ACRabbitCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void HandleDead() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
private:
	bool bCanAttack;
};
