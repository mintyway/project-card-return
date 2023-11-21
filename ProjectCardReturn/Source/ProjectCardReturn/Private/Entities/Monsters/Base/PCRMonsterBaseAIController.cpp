// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

DEFINE_LOG_CATEGORY(PCRLogMonsterBaseAIController);

const FName APCRMonsterBaseAIController::TargetKey(TEXT("Target"));
const FName APCRMonsterBaseAIController::IsDetectedKey(TEXT("IsDetected"));

APCRMonsterBaseAIController::APCRMonsterBaseAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPCRMonsterDataAsset> DA_MonsterDataAsset(TEXT("/Script/ProjectCardReturn.PCRMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_MonsterDataAsset.Succeeded())
	{
		MonsterDataAsset = DA_MonsterDataAsset.Object;
	}
}

void APCRMonsterBaseAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APCRMonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void APCRMonsterBaseAIController::BeginPlay()
{
	Super::BeginPlay();

	// 만약 처음부터 월드에 몬스터가 배치되었을 경우 타겟이 미할당 되기때문에 재할당합니다.
	// TODO: 널체크 완화 필요
	// SetTarget();
}

void APCRMonsterBaseAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 타겟을 계속 바라보도록 하는 코드입니다.
	APCRMonsterBaseCharacter* ControllingMonster = Cast<APCRMonsterBaseCharacter>(GetPawn());
	check(ControllingMonster);
	if (ControllingMonster->IsAlive())
	{
		check(GetBlackboardComponent());
		const AActor* Target = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(APCRMonsterBaseAIController::TargetKey));
			
		check(Target);
		const FVector TargetLocation = Target->GetActorLocation();
		const FVector TargetDirection = (TargetLocation - ControllingMonster->GetActorLocation()).GetSafeNormal();

		const FRotator CurrentRotation = ControllingMonster->GetActorRotation();
		const FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetDirection).Rotator();

		const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 10.f);

		ControllingMonster->SetActorRotation(NewRotation);
	}
}

void APCRMonsterBaseAIController::SetTarget()
{
	check(GetBlackboardComponent());
	check(GetWorld());
	const AController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);
	GetBlackboardComponent()->SetValueAsObject(APCRMonsterBaseAIController::TargetKey, PlayerController->GetPawn());
}
