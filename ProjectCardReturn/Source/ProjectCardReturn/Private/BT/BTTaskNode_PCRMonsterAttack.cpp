// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTaskNode_PCRMonsterAttack.h"

#include "AIController.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

UBTTaskNode_PCRMonsterAttack::UBTTaskNode_PCRMonsterAttack()
{
	NodeName = TEXT("Attack");
	MonsterTask = EMonsterTask::Attack;
}

EBTNodeResult::Type UBTTaskNode_PCRMonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	switch (MonsterTask)
	{
	case EMonsterTask::Attack:
		{
			check(OwnerComp.GetAIOwner());
			APCRMonsterBaseCharacter* Monster = Cast<APCRMonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());

			check(Monster);
			UAnimInstance* AnimInstance = Monster->GetMesh()->GetAnimInstance();
			
			Monster->Attack();

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
