// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTService_PCRMonsterDetect.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

UBTService_PCRMonsterDetect::UBTService_PCRMonsterDetect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_PCRMonsterDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APCRMonsterBaseCharacter* ControllingMonster = OwnerComp.GetAIOwner() ? Cast<APCRMonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn()) : nullptr;
	RETURN_IF_INVALID(ControllingMonster);

	RETURN_IF_INVALID(OwnerComp.GetBlackboardComponent());
	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APCRMonsterBaseAIController::TargetKey));
	RETURN_IF_INVALID(TargetActor);

	const float Distance = TargetActor->GetDistanceTo(ControllingMonster);
	const float AttackRange = ControllingMonster->GetAttackRange();

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(APCRMonsterBaseAIController::IsDetectedKey, Distance <= AttackRange);
}
