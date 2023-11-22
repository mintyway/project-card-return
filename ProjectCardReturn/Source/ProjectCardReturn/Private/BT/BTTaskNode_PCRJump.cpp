// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BTTaskNode_PCRJump.h"

#include "AIController.h"
#include "Entities/Monsters/Rabbit/PCRRabbitAnimInstance.h"
#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"
#include "GameFramework/Character.h"

UBTTaskNode_PCRJump::UBTTaskNode_PCRJump()
{
	NodeName = TEXT("Jump");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_PCRJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	check(OwnerComp.GetAIOwner());
	APCRRabbitCharacter* Rabbit = Cast<APCRRabbitCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	check(Rabbit)
	Rabbit->RabbitJump();

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_PCRJump::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
