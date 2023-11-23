// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTaskNode_PCRRabbitJump.h"

#include "AIController.h"
#include "Entities/Monsters/Rabbit/PCRRabbitAnimInstance.h"
#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"
#include "GameFramework/Character.h"

UBTTaskNode_PCRRabbitJump::UBTTaskNode_PCRRabbitJump()
{
	NodeName = TEXT("Jump");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_PCRRabbitJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	check(OwnerComp.GetAIOwner());
	APCRRabbitCharacter* Rabbit = Cast<APCRRabbitCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	check(Rabbit)
	Rabbit->RabbitJump();

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_PCRRabbitJump::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	check(OwnerComp.GetAIOwner());
	const APCRRabbitCharacter* Rabbit = Cast<APCRRabbitCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	check(Rabbit);
	const UPCRRabbitAnimInstance* RabbitAnimInstance = Cast<UPCRRabbitAnimInstance>(Rabbit->GetMesh()->GetAnimInstance());
	
	if (RabbitAnimInstance->GetCanJump())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
