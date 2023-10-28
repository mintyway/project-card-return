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

void APCRMonsterBaseAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(MonsterDataAsset);
}

void APCRMonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CachedMonsterCharacter = Cast<APCRMonsterBaseCharacter>(InPawn);
	check(CachedMonsterCharacter);

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
		RETURN_IF_INVALID(ControllingMonster);
		if (ControllingMonster->IsAlive())
		{
			RETURN_IF_INVALID(IsValid(GetBlackboardComponent()));
			const AActor* Target = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(APCRMonsterBaseAIController::TargetKey));
			RETURN_IF_INVALID(Target)
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

	GetWorldTimerManager().ClearTimer(StunTimerHandle);
	FTimerDelegate StunTimerDelegate;
	StunTimerDelegate.BindUObject(this, &APCRMonsterBaseAIController::StunTimerCallback);
	GetWorldTimerManager().SetTimer(StunTimerHandle, StunTimerDelegate, StunTime, false);
}

void APCRMonsterBaseAIController::SetTarget()
{
	RETURN_IF_INVALID(IsValid(GetBlackboardComponent()));
	const AController* PlayerController = IsValid(GetWorld()) ? GetWorld()->GetFirstPlayerController() : nullptr;
	RETURN_IF_INVALID(IsValid(PlayerController));
	GetBlackboardComponent()->SetValueAsObject(APCRMonsterBaseAIController::TargetKey, PlayerController->GetPawn());
}

void APCRMonsterBaseAIController::StunTimerCallback()
{
	bIsStunned = false;
	if (CachedMonsterCharacter->IsAlive())
	{
		BrainComponent->RestartLogic();
	}
}
