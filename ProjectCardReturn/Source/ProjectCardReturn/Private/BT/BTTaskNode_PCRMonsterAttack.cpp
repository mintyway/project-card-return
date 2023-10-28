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

	//CachedOwnerComp = &OwnerComp;

	switch (MonsterTask)
	{
	case EMonsterTask::Attack:
		{
			check(OwnerComp.GetAIOwner());
			APCRMonsterBaseCharacter* Monster = Cast<APCRMonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
		
			/*FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindLambda(
				[&]()
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			);*/

			check(Monster);
			UAnimInstance* AnimInstance = Monster->GetMesh()->GetAnimInstance();
			
			/*check(AnimInstance);
			if (!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &UBTTaskNode_PCRMonsterAttack::HandleAttackEnded))
			{
				AnimInstance->OnMontageEnded.AddDynamic(this, &UBTTaskNode_PCRMonsterAttack::HandleAttackEnded);
			}*/
			
			Monster->Attack();

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}

/*void UBTTaskNode_PCRMonsterAttack::HandleAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (const APCRMonsterBaseCharacter* Monster = Cast<APCRMonsterBaseCharacter>(CachedOwnerComp->GetAIOwner()->GetPawn()))
	{
		if (UAnimInstance* AnimInstance = Monster->GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTaskNode_PCRMonsterAttack::HandleAttackEnded);
		}
	}
	
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}*/
