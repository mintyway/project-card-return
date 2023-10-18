// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Monsters/MeleeSoldier/PCRSoldierBaseCharacter.h"
#include "PCRMeleeSoldierCharacter.generated.h"

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
};
