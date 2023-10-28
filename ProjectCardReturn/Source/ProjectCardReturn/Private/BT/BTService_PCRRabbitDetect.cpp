// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTService_PCRRabbitDetect.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Monsters/Rabbit/PCRRabbitAIController.h"
#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"

void UBTService_PCRRabbitDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	check(OwnerComp.GetAIOwner());
	const APCRRabbitCharacter* ControllingMonster = Cast<APCRRabbitCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(ControllingMonster);

	check(OwnerComp.GetBlackboardComponent());
	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APCRRabbitAIController::TargetKey));
	check(TargetActor);

	const float Distance = ControllingMonster->GetDistanceTo(TargetActor);
	const float GimmickMoveRange = ControllingMonster->GetGimmickMoveRange();
	const bool bGimmickMove = Distance <= GimmickMoveRange;
	const bool bJump = FMath::RandRange(0.f, 1.f) < ControllingMonster->GetJumpProbability();
	const bool bWaitAndJump = FMath::RandRange(0.f, 1.f) < ControllingMonster->GetWaitAndJumpProbability();
	const bool bMoveRightDiagonal = FMath::RandRange(0.f, 1.f) < ControllingMonster->GetMoveRightDiagonalProbability();
	const bool bMoveLeftDiagonal = FMath::RandRange(0.f, 1.f) < ControllingMonster->GetMoveLeftDiagonalProbability();

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(APCRRabbitAIController::IsGimmickMoveKey, bGimmickMove);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(APCRRabbitAIController::IsJumpKey, bJump);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(APCRRabbitAIController::IsWaitAndJumpKey, bWaitAndJump);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(APCRRabbitAIController::IsMoveRightDiagonalKey, bMoveRightDiagonal);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(APCRRabbitAIController::IsMoveLeftDiagonalKey, bMoveLeftDiagonal);
}
