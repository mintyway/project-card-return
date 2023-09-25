// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsTargetInMonsterAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UBTDecorator_IsTargetInMonsterAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsTargetInMonsterAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
