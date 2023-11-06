// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"
#include "PCRSoldierBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRSoldierBaseAIController : public APCRMonsterBaseAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
