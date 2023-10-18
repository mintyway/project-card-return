// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTService_PCRSoldierDetect.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

UBTService_PCRSoldierDetect::UBTService_PCRSoldierDetect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_PCRSoldierDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APCRMonsterBaseCharacter* ControllingMonster = OwnerComp.GetAIOwner() ? Cast<APCRMonsterBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn()) : nullptr;
	RETURN_IF_INVALID(ControllingMonster);

	RETURN_IF_INVALID(OwnerComp.GetBlackboardComponent());
	auto TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APCRMonsterBaseAIController::TargetKey));
	RETURN_IF_INVALID(TargetActor);

	float Distance = TargetActor->GetDistanceTo(ControllingMonster);
	float AttackRange = ControllingMonster->GetAttackRange();

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(APCRMonsterBaseAIController::DetectKey, Distance <= AttackRange);
}
