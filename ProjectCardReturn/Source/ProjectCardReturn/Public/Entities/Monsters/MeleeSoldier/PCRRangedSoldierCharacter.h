// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Monsters/MeleeSoldier/PCRSoldierBaseCharacter.h"
#include "PCRRangedSoldierCharacter.generated.h"

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
};
