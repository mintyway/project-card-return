// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_PCRRabbitJump.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UBTTaskNode_PCRRabbitJump : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_PCRRabbitJump();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
