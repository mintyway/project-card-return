// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTaskNode_PCRJump.h"

#include "AIController.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UBTTaskNode_PCRJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	check(OwnerComp.GetAIOwner());
	ACharacter* Character = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

	check(Character)
	Character->Jump();

	return EBTNodeResult::Succeeded;
}
