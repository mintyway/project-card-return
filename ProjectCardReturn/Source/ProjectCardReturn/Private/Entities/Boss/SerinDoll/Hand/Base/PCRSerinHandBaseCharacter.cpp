// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/Base/PCRSerinHandBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/SerinDoll/PCRSerinCharacter.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinPrimaryDataAsset.h"

#include "AIController.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogSerinHandBaseCharacter);

APCRSerinHandBaseCharacter::APCRSerinHandBaseCharacter()
	: CurrentSerinState(ESerinState::Invalid),
	  bUsePredictiveChase(false)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = nullptr;
	// TODO: 모델링 구현시 제거
	DummyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyStaticMeshComponent"));
	if (SerinDataAsset)
	{
		DummyStaticMeshComponent->SetupAttachment(GetCapsuleComponent());
		DummyStaticMeshComponent->SetStaticMesh(SerinDataAsset->CubeDummyMesh);
	}
	SetActorEnableCollision(false);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = 0.f;
	}
}

void APCRSerinHandBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CachedSerinCharacter = Cast<APCRSerinCharacter>(GetOwner());
	check(CachedSerinCharacter);
}

void APCRSerinHandBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APCRSerinHandBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSerinHandBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentSerinState)
	{
		case ESerinState::Idle:
		{
			break;
		}
		case ESerinState::Move:
		{
			HandleMove(DeltaTime);
			break;
		}
		case ESerinState::Chase:
		{
			HandleChase(DeltaTime);
			break;
		}
		case ESerinState::BasicChase:
		{
			HandleBasicChase(DeltaTime);
			break;
		}
		case ESerinState::Rock:
		{
			HandleRock(DeltaTime);
			break;
		}
		case ESerinState::Paper:
		{
			break;
		}
		case ESerinState::Scissors:
		{
			break;
		}
		case ESerinState::Invalid:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void APCRSerinHandBaseCharacter::SetTarget(AActor* TargetActor)
{
	CachedTarget = TargetActor;
	check(CachedTarget);
}

/**
 * 입력된 좌표까지 이동시킵니다.
 */
void APCRSerinHandBaseCharacter::Move(const FVector& InLocation, bool bUseReset)
{
	if (bUseReset)
	{
		StateReset();
	}

	MoveLocation = InLocation;
	CurrentSerinState = ESerinState::Move;
}

/**
 * 설정된 타겟에 일정 거리를 두고 계속 추적하는 기본 체이스 상태입니다.
 */
void APCRSerinHandBaseCharacter::BasicChase(bool bUseReset)
{
	if (bUseReset)
	{
		StateReset();
	}

	CurrentSerinState = ESerinState::BasicChase;
}

/**
 * 설정된 타겟을 추적합니다. 접근 시 기본 상태로 돌아갑니다. 만약 델리게이트를 바인드해준다면 접근 이후 원하는 행동을 동작시킬 수 있습니다.
 * 기본 체이스보다 훨씬 빠른속도로 접근합니다. 보통 공격 연계를 위해 사용합니다.
 */
void APCRSerinHandBaseCharacter::Chase(bool bUsePredictive, bool bUseReset)
{
	if (bUseReset)
	{
		StateReset();
	}

	CurrentSerinState = ESerinState::Chase;
	bUsePredictiveChase = bUsePredictive;
}

/**
 * 타겟에게 빠르게 접근 한 뒤에 주먹으로 내려칩니다.
 */
void APCRSerinHandBaseCharacter::RockAttack(bool bUseReset)
{
	if (bUseReset)
	{
		StateReset();
	}

	OnChaseEnd.BindUObject(this, &APCRSerinHandBaseCharacter::RockCallback);
	// RockAttackPredictiveTime = 
	Chase(true, false);
}

void APCRSerinHandBaseCharacter::PaperAttack(bool bUseReset)
{
	if (bUseReset)
	{
		StateReset();
	}
	
}

void APCRSerinHandBaseCharacter::Scissors(bool bUseReset)
{
	if (bUseReset)
	{
		StateReset();
	}
	
}

/**
 * 진행중인 동작을 끝내지 않고 다른 동작을 시작할때 버그를 방지하는 리셋 함수입니다. bool값으로 놓은 이유는 연계되는 동작인 경우까지 초기화하면 안 되기 때문입니다. 따라서 기본적으로는 초기화를 하지만 연계되는 경우는 초기화하지 않는 방법으로 사용하면 됩니다.
 */
void APCRSerinHandBaseCharacter::StateReset()
{
	OnChaseEnd.Unbind();
	bUsePredictiveChase = false;
}

void APCRSerinHandBaseCharacter::HandleMove(float DeltaTime)
{
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), MoveLocation, DeltaTime, ParameterDataAsset->SerinMoveSpeed);
	SetActorLocation(NewLocation);

	const float DistSquare2D = FVector::DistSquared2D(NewLocation, MoveLocation);
	if (DistSquare2D <= FMath::Square(CachedSerinCharacter->ContactDistance))
	{
		CurrentSerinState = ESerinState::BasicChase;
	}
}

void APCRSerinHandBaseCharacter::HandleBasicChase(float DeltaTime)
{
	// 각 손의 해당 함수도 참고해주세요.
	ChaseLocation.Z = CachedSerinCharacter->DefaultFloatingHeight;
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), ChaseLocation, DeltaTime, ParameterDataAsset->SerinBasicChaseSpeed);
	SetActorLocation(NewLocation);
}

void APCRSerinHandBaseCharacter::HandleChase(float DeltaTime)
{
	if (bUsePredictiveChase)
	{
		TIME_CHECK_START(0);
		ChaseLocation = CachedTarget->GetActorLocation();
		FVector RockAttackLocation = GetActorLocation();
		RockAttackLocation.Z = CachedSerinCharacter->GetLiftHeight();
		const float RockAttackPredictiveTime = FVector::Dist(GetActorLocation(), RockAttackLocation) / ParameterDataAsset->SerinRockSpeed;
		UE_LOG(PCRLogSerinHandBaseCharacter, Warning, TEXT("%f"), RockAttackPredictiveTime);
		const FVector Velocity = CachedTarget->GetVelocity() * RockAttackPredictiveTime;
		ChaseLocation += Velocity;
		ChaseLocation.Z = CachedSerinCharacter->DefaultFloatingHeight;
		TIME_CHECK_END(0);
	}
	else
	{
		ChaseLocation = CachedTarget->GetActorLocation();
		ChaseLocation.Z = CachedSerinCharacter->DefaultFloatingHeight;
	}

	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), ChaseLocation, DeltaTime, ParameterDataAsset->SerinChaseSpeed);
	SetActorLocation(NewLocation);

	const float DistSquare2D = FVector::DistSquared2D(NewLocation, ChaseLocation);
	if (DistSquare2D <= FMath::Square(CachedSerinCharacter->ContactDistance))
	{
		if (OnChaseEnd.IsBound())
		{
			OnChaseEnd.Execute();
			OnChaseEnd.Unbind();
		}
		else
		{
			CurrentSerinState = ESerinState::BasicChase;
		}
	}
}

void APCRSerinHandBaseCharacter::RockCallback()
{
	CurrentSerinState = ESerinState::Rock;
	UE_LOG(PCRLogSerinHandBaseCharacter, Log, TEXT("주먹 공격 시작"));
	ChaseLocation = GetActorLocation();
	ChaseLocation.Z = CachedSerinCharacter->GetLiftHeight();
}

void APCRSerinHandBaseCharacter::HandleRock(float DeltaTime)
{
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), ChaseLocation, DeltaTime, ParameterDataAsset->SerinRockSpeed);
	SetActorLocation(NewLocation);
	const float DistSquare = FVector::DistSquared(NewLocation, ChaseLocation);
	if (DistSquare <= FMath::Square(10.f))
	{
		UE_LOG(PCRLogSerinHandBaseCharacter, Log, TEXT("주먹 공격 끝"));
		CurrentSerinState = ESerinState::BasicChase;
	}
}
