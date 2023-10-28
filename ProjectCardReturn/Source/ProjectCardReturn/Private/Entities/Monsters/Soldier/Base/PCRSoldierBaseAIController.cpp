// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/Base/PCRSoldierBaseAIController.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"

void APCRSoldierBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	check(MonsterDataAsset);
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(MonsterDataAsset->SoldierBlackBoard, BlackboardComponent))
	{
		if (!RunBehaviorTree(MonsterDataAsset->SoldierBehaviorTree))
		{
			UE_LOG(PCRLogMonsterBaseAIController, Error, TEXT("AIController couldn't run behavior tree!"));
		}

		SetTarget();
	}
}
