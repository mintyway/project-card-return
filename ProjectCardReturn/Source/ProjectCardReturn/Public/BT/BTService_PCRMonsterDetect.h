// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "BehaviorTree/BTService.h"
#include "BTService_PCRMonsterDetect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UBTService_PCRMonsterDetect : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_PCRMonsterDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
