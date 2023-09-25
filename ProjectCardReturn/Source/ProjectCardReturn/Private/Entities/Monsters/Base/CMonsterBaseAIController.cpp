// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Base/CMonsterBaseAIController.h"

#include "Entities/Monsters/Base/CMonsterDataAsset.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Monsters/Base/CMonsterBaseCharacter.h"

const FName ACMonsterBaseAIController::TargetKey(TEXT("Target"));

ACMonsterBaseAIController::ACMonsterBaseAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UCMonsterDataAsset> DA_Monster(TEXT("/Script/ProjectCardReturn.CMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}
}

void ACMonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RETURN_IF_INVALID(IsValid(MonsterDataAsset));
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(GetMonsterDataAsset()->GetDefaultBlackBoard(), BlackboardComponent))
	{
		if (!RunBehaviorTree(GetMonsterDataAsset()->GetDefaultBehaviorTree()))
		{
			UE_LOG(RuntimeLog, Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void ACMonsterBaseAIController::BeginPlay()
{
	Super::BeginPlay();

	RETURN_IF_INVALID(IsValid(GetBlackboardComponent()));
	AController* PlayerController = IsValid(GetWorld()) ? GetWorld()->GetFirstPlayerController() : nullptr;
	RETURN_IF_INVALID(PlayerController);
	GetBlackboardComponent()->SetValueAsObject(TargetKey, PlayerController->GetPawn());
}

void ACMonsterBaseAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 타겟을 계속 바라보도록 하는 코드입니다.
	ACMonsterBaseCharacter* ControllingMonster = Cast<ACMonsterBaseCharacter>(GetPawn());
	RETURN_IF_INVALID(ControllingMonster);
	if (ControllingMonster->IsAlive())
	{
		RETURN_IF_INVALID(IsValid(GetBlackboardComponent()));
		AActor* Target = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetKey));
		RETURN_IF_INVALID(Target)
		FVector TargetLocation = Target->GetActorLocation();
		FVector TargetDirection = (TargetLocation - ControllingMonster->GetActorLocation()).GetSafeNormal();
		ControllingMonster->SetActorRotation(FRotationMatrix::MakeFromX(TargetDirection).Rotator());
	}
}
