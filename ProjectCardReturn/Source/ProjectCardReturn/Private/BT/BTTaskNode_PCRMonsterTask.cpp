// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTaskNode_PCRMonsterTask.h"

#include "AIController.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

UBTTaskNode_PCRMonsterTask::UBTTaskNode_PCRMonsterTask()
{
	MonsterTask = EMonsterTask::Attack;
}

EBTNodeResult::Type UBTTaskNode_PCRMonsterTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	switch (MonsterTask)
	{
		case EMonsterTask::Attack:
		{
			RETURN_IF_INVALID(OwnerComp.GetAIOwner(), EBTNodeResult::Failed);
			APCRMonsterBaseCharacter* Monster = Cast<APCRMonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
			RETURN_IF_INVALID(Monster, EBTNodeResult::Failed);
			Monster->Attack();
			
			break;
		}
		default:
		{
			break;
		}
	}

	return EBTNodeResult::Succeeded;
}
