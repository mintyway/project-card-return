// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTaskNode_PCRMonsterAttack.h"

#include "AIController.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAnimInstance.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

UBTTaskNode_PCRMonsterAttack::UBTTaskNode_PCRMonsterAttack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_PCRMonsterAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	check(OwnerComp.GetAIOwner());
	APCRMonsterBaseCharacter* Monster = Cast<APCRMonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	check(Monster);
	Monster->Attack();
	return EBTNodeResult::InProgress;
}

void UBTTaskNode_PCRMonsterAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	check(OwnerComp.GetAIOwner());
	const APCRMonsterBaseCharacter* Monster = Cast<APCRMonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(Monster);
	const UPCRMonsterBaseAnimInstance* MonsterAnimInstance = Cast<UPCRMonsterBaseAnimInstance>(Monster->GetMesh()->GetAnimInstance());
	
	if (MonsterAnimInstance->GetCanAttack())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
