// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTDecorator_IsTargetInMonsterAttackRange.h"

#include "Entities/Monsters/Base/CMonsterBaseCharacter.h"
#include "Entities/Monsters/Base/CMonsterBaseAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsTargetInMonsterAttackRange::UBTDecorator_IsTargetInMonsterAttackRange()
{
	NodeName = TEXT("IsTargetInMonsterAttackRange");
}

bool UBTDecorator_IsTargetInMonsterAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ACMonsterBaseCharacter* ControllingMonster = OwnerComp.GetAIOwner() ? Cast<ACMonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn()) : nullptr;
	RETURN_IF_INVALID(ControllingMonster, false);

	RETURN_IF_INVALID(OwnerComp.GetBlackboardComponent(), false);
	auto TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACMonsterBaseAIController::TargetKey));
	RETURN_IF_INVALID(TargetActor, false);
	
	float DistanceSquared = (TargetActor->GetActorLocation() - ControllingMonster->GetActorLocation()).SizeSquared();
	
	float AttackRange = ControllingMonster->GetAttackRange();
	float AttackRangeSquared = AttackRange * AttackRange;

	return DistanceSquared <= AttackRangeSquared;
}

