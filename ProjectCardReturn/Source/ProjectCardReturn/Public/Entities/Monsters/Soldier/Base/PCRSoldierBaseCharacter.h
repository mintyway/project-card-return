// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"
#include "PCRSoldierBaseCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PCRLogMeleeSoldierCharacter, Log, All);

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
};
