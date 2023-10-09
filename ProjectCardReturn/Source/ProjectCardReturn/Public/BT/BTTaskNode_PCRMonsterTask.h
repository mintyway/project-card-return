// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_PCRMonsterTask.generated.h"

UENUM()
enum class EMonsterTask
{
	Attack
};
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UBTTaskNode_PCRMonsterTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_PCRMonsterTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	EMonsterTask MonsterTask;
};