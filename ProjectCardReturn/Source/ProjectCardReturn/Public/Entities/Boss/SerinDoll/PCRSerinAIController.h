// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "AIController.h"
#include "PCRSerinAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCRSerinAIController : public AAIController
{
	GENERATED_BODY()

public:
	APCRSerinAIController();

protected:
	virtual void Tick(float DeltaSeconds) override;
};
