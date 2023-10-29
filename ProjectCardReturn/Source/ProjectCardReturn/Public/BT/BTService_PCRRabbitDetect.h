// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "BT/BTService_PCRMonsterDetect.h"
#include "BTService_PCRRabbitDetect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UBTService_PCRRabbitDetect : public UBTService_PCRMonsterDetect
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
