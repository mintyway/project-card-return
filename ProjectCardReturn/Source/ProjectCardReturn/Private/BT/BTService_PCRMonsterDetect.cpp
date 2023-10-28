// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTService_PCRMonsterDetect.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

UBTService_PCRMonsterDetect::UBTService_PCRMonsterDetect()
{
	NodeName = TEXT("Detect");
}

void UBTService_PCRMonsterDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	check(OwnerComp.GetAIOwner());
	const APCRMonsterBaseCharacter* ControllingMonster = Cast<APCRMonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(ControllingMonster);

	check(OwnerComp.GetBlackboardComponent());
	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APCRMonsterBaseAIController::TargetKey));
	check(TargetActor);

	const float Distance = ControllingMonster->GetDistanceTo(TargetActor);
	const float AttackRange = ControllingMonster->GetAttackRange();
	const bool bDetected = Distance <= AttackRange;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(APCRMonsterBaseAIController::IsDetectedKey, bDetected);
}
