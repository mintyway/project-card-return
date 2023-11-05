// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "AIController.h"
#include "PCRSerinDollAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRSerinDollAIController : public AAIController
{
	GENERATED_BODY()

public:
	APCRSerinDollAIController();

protected:
	virtual void Tick(float DeltaSeconds) override;
};
