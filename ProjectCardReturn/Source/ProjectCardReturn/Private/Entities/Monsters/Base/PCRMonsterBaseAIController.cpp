// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Base/PCRMonsterBaseAIController.h"

#include "BrainComponent.h"
#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Entities/Monsters/Base/PCRMonsterBaseAnimInstance.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

DEFINE_LOG_CATEGORY(PCRLogMonsterBaseAIController);

const FName APCRMonsterBaseAIController::TargetKey(TEXT("Target"));
const FName APCRMonsterBaseAIController::IsDetectedKey(TEXT("IsDetected"));

APCRMonsterBaseAIController::APCRMonsterBaseAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// TODO: 파라미터화 필요 및 몬스터 베이스 클래스로 이동 필요
	bIsStunned = false;

	static ConstructorHelpers::FObjectFinder<UPCRMonsterDataAsset> DA_Monster(TEXT("/Script/ProjectCardReturn.PCRMonsterDataAsset'/Game/DataAssets/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}
}

void APCRMonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	check(MonsterDataAsset);
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(MonsterDataAsset->DefaultBlackBoard, BlackboardComponent))
	{
		if (!RunBehaviorTree(MonsterDataAsset->DefaultBehaviorTree))
		{
			UE_LOG(PCRLogMonsterBaseAIController, Error, TEXT("AIController couldn't run behavior tree!"));
		}

		SetTarget();
	}
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
	if (!bIsStunned)
	{
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
}

void APCRMonsterBaseAIController::ApplyStun(float StunTime)
{
	bIsStunned = true;

	BrainComponent->StopLogic("Stunned");
	UE_LOG(PCRLogMonsterBaseAIController, Log, TEXT("%s가 %f.1초간 스턴에 빠집니다."), *GetPawn()->GetName(), StunTime);

	GetWorldTimerManager().ClearTimer(StunTimerHandle);
	FTimerDelegate StunTimerDelegate;
	StunTimerDelegate.BindUObject(this, &APCRMonsterBaseAIController::StunTimerCallback);
	GetWorldTimerManager().SetTimer(StunTimerHandle, StunTimerDelegate, StunTime, false);
}

void APCRMonsterBaseAIController::SetTarget()
{
	check(GetBlackboardComponent());
	check(GetWorld());
	const AController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);
	GetBlackboardComponent()->SetValueAsObject(APCRMonsterBaseAIController::TargetKey, PlayerController->GetPawn());
}

void APCRMonsterBaseAIController::StunTimerCallback()
{
	bIsStunned = false;
	BrainComponent->RestartLogic();
	UE_LOG(PCRLogMonsterBaseAIController, Log, TEXT("%s의 스턴이 풀렸습니다."), *GetPawn()->GetName());
}
