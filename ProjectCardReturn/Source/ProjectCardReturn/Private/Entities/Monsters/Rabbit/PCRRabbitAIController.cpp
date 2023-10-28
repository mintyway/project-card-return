// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/PCRRabbitAIController.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"

const FName APCRRabbitAIController::IsGimmickMoveKey(TEXT("IsGimmickMove"));
const FName APCRRabbitAIController::IsJumpKey(TEXT("IsJump"));
const FName APCRRabbitAIController::IsWaitAndJumpKey(TEXT("IsWaitAndJump"));
const FName APCRRabbitAIController::IsMoveRightDiagonalKey(TEXT("IsMoveRightDiagonal"));
const FName APCRRabbitAIController::IsMoveLeftDiagonalKey(TEXT("IsMoveLeftDiagonal"));
const FName APCRRabbitAIController::RightDiagonalKey(TEXT("RightDiagonal"));
const FName APCRRabbitAIController::LeftDiagonalKey(TEXT("LeftDiagonal"));

void APCRRabbitAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	check(MonsterDataAsset);
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(MonsterDataAsset->RabbitBlackBoard, BlackboardComponent))
	{
		if (!RunBehaviorTree(MonsterDataAsset->RabbitBehaviorTree))
		{
			UE_LOG(PCRLogMonsterBaseAIController, Error, TEXT("AIController couldn't run behavior tree!"));
		}

		SetTarget();
	}
}
