// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_PCRMonsterAttack.generated.h"

UENUM()
enum class EMonsterTask
{
	Attack
};
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UBTTaskNode_PCRMonsterAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_PCRMonsterAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	//UPROPERTY()
	//TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	//UFUNCTION()
	//void HandleAttackEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere)
	EMonsterTask MonsterTask;
};
