// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Hand/Base/PCRSerinDollHandBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/SerinDoll/PCRSerinDollCharacter.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"

#include "AIController.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "Game/PCRParameterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogSerinHandBaseCharacter);

APCRSerinDollHandBaseCharacter::APCRSerinDollHandBaseCharacter()
	: CurrentSerinState(ESerinState::Invalid), CurrentScissorsState(EScissorsState::Invalid),
	  bUsePredictiveChase(false),
	  ScissorsAttackCount(0), ScissorsAttackMaxCount(3), ScissorsCoolDown(1.5f), ScissorsCoolDownElapsedTime(0.f)
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = nullptr;

	SetActorRotation(FRotator(0.0, 180.0, 0.0));

	if (GetMesh())
	{
		GetMesh()->SetRelativeLocation(FVector(-340.0, 0.0, 0.0));
		GetMesh()->SetupAttachment(GetCapsuleComponent());
	}
	SetActorEnableCollision(false);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = 0.f;
	}
}

void APCRSerinDollHandBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CachedSerinDollCharacter = Cast<APCRSerinDollCharacter>(GetOwner());
	check(CachedSerinDollCharacter);
}

void APCRSerinDollHandBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APCRSerinDollHandBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCRSerinDollHandBaseCharacter::Tick(float DeltaTime)
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
			HandlePaper(DeltaTime);
			break;
		}
		case ESerinState::Scissors:
		{
			HandleScissors(DeltaTime);
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

void APCRSerinDollHandBaseCharacter::SetTarget(AActor* TargetActor)
{
	CachedTarget = TargetActor;
	check(CachedTarget);
}

/**
 * 입력된 좌표까지 이동시킵니다. 델리게이트로 연계 가능합니다.
 */
void APCRSerinDollHandBaseCharacter::Move(const FVector& InLocation, bool bUseReset)
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
void APCRSerinDollHandBaseCharacter::BasicChase(bool bUseReset)
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
void APCRSerinDollHandBaseCharacter::Chase(bool bUsePredictive, bool bUseReset)
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
void APCRSerinDollHandBaseCharacter::RockAttack(bool bUseReset)
{
	if (bUseReset)
	{
		StateReset();
	}

	OnChaseEnd.BindUObject(this, &APCRSerinDollHandBaseCharacter::RockCallback);
	// RockAttackPredictiveTime = 
	Chase(false, false);
}

void APCRSerinDollHandBaseCharacter::PaperAttack(bool bUseReset)
{
	if (bUseReset)
	{
		StateReset();
	}

	FVector NewLocation = GetActorLocation();
	NewLocation.X = CachedTarget->GetActorLocation().X;
	NewLocation.Z = CachedTarget->GetActorLocation().Z;
	OnMoveEnd.BindUObject(this, &APCRSerinDollHandBaseCharacter::PaperCallback);
	Move(NewLocation, false);
}

void APCRSerinDollHandBaseCharacter::ScissorsAttack(bool bUseReset)
{
	if (bUseReset)
	{
		StateReset();
	}

	FVector NewLocation = GetActorLocation();
	NewLocation.X = CachedTarget->GetActorLocation().X;
	NewLocation.Z = CachedTarget->GetActorLocation().Z;
	OnMoveEnd.BindUObject(this, &APCRSerinDollHandBaseCharacter::ScissorsCallback);
	Move(NewLocation, false);
}

/**
 * 진행중인 동작을 끝내지 않고 다른 동작을 시작할때 버그를 방지하는 리셋 함수입니다. bool값으로 놓은 이유는 연계되는 동작인 경우까지 초기화하면 안 되기 때문입니다. 따라서 기본적으로는 초기화를 하지만 연계되는 경우는 초기화하지 않는 방법으로 사용하면 됩니다.
 */
void APCRSerinDollHandBaseCharacter::StateReset()
{
	OnChaseEnd.Unbind();
	OnMoveEnd.Unbind();

	bUsePredictiveChase = false;

	CurrentScissorsState = EScissorsState::Invalid;
}

void APCRSerinDollHandBaseCharacter::HandleMove(float DeltaTime)
{
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), MoveLocation, DeltaTime, ParameterDataAsset->SerinMoveSpeed);
	SetActorLocation(NewLocation);

	const float DistSquare = FVector::DistSquared(NewLocation, MoveLocation);
	if (DistSquare <= FMath::Square(CachedSerinDollCharacter->ContactDistance))
	{
		if (OnMoveEnd.IsBound())
		{
			OnMoveEnd.Execute();
			OnMoveEnd.Unbind();
		}
		else
		{
			CurrentSerinState = ESerinState::BasicChase;
		}
	}
}

void APCRSerinDollHandBaseCharacter::HandleBasicChase(float DeltaTime)
{
	// 각 손의 해당 함수도 참고해주세요.
	ChaseLocation.Z = CachedSerinDollCharacter->GetHandWorldHeight();
	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), ChaseLocation, DeltaTime, /*ParameterDataAsset->SerinBasicChaseSpeed*/ 3.f);
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), FRotator(0.0, 180.0, 0.0), DeltaTime, /*ParameterDataAsset->SerinBasicChaseSpeed*/ 3.f);
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void APCRSerinDollHandBaseCharacter::HandleChase(float DeltaTime)
{
	if (bUsePredictiveChase)
	{
		TIME_CHECK_START(0);
		ChaseLocation = CachedTarget->GetActorLocation();
		FVector RockAttackLocation = GetActorLocation();
		RockAttackLocation.Z = CachedSerinDollCharacter->GetLiftHeight();
		const float RockAttackPredictiveTime = FVector::Dist(GetActorLocation(), RockAttackLocation) / ParameterDataAsset->SerinRockSpeed;
		UE_LOG(PCRLogSerinHandBaseCharacter, Warning, TEXT("%f"), RockAttackPredictiveTime);
		const FVector Velocity = CachedTarget->GetVelocity() * RockAttackPredictiveTime;
		ChaseLocation += Velocity;
		ChaseLocation.Z = CachedSerinDollCharacter->GetHandWorldHeight();
		TIME_CHECK_END(0);
	}
	else
	{
		ChaseLocation = CachedTarget->GetActorLocation();
		ChaseLocation.Z = CachedSerinDollCharacter->GetHandWorldHeight();
	}

	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), ChaseLocation, DeltaTime, ParameterDataAsset->SerinChaseSpeed);
	SetActorLocation(NewLocation);

	const float DistSquare2D = FVector::DistSquared2D(NewLocation, ChaseLocation);
	if (DistSquare2D <= FMath::Square(CachedSerinDollCharacter->ContactDistance))
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

void APCRSerinDollHandBaseCharacter::RockCallback()
{
	CurrentSerinState = ESerinState::Rock;
	UE_LOG(PCRLogSerinHandBaseCharacter, Log, TEXT("바위 공격 시작"));
	ChaseLocation = GetActorLocation();
	ChaseLocation.Z = CachedSerinDollCharacter->GetLiftHeight();
}

void APCRSerinDollHandBaseCharacter::HandleRock(float DeltaTime)
{
	// TODO: 파라미터화 필요
	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), ChaseLocation, DeltaTime, /*ParameterDataAsset->SerinRockSpeed*/ 15.f);
	SetActorLocation(NewLocation);

	const float DistSquare = FVector::DistSquared(NewLocation, ChaseLocation);
	if (DistSquare <= FMath::Square(10.f))
	{
		UE_LOG(PCRLogSerinHandBaseCharacter, Log, TEXT("바위 공격 끝"));
		CurrentSerinState = ESerinState::BasicChase;
	}
}

void APCRSerinDollHandBaseCharacter::PaperCallback()
{
	CurrentSerinState = ESerinState::Paper;
	UE_LOG(PCRLogSerinHandBaseCharacter, Log, TEXT("보 공격 시작"));
	const FVector Direction = (CachedTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	// TODO: 파라미터화 필요
	ChaseLocation = GetActorLocation() + (Direction * 1500.f);
}

void APCRSerinDollHandBaseCharacter::HandlePaper(float DeltaTime)
{
	// TODO: 파라미터화 필요
	const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), ChaseLocation, DeltaTime, 10.f);
	SetActorLocation(NewLocation);

	const float DistSquare = FVector::DistSquared(NewLocation, ChaseLocation);
	if (DistSquare <= FMath::Square(10.f))
	{
		UE_LOG(PCRLogSerinHandBaseCharacter, Log, TEXT("보 공격 끝"));
		CurrentSerinState = ESerinState::BasicChase;
	}
}

void APCRSerinDollHandBaseCharacter::ScissorsCallback()
{
	CurrentScissorsState = EScissorsState::Stay;
	CurrentSerinState = ESerinState::Scissors;
	UE_LOG(PCRLogSerinHandBaseCharacter, Log, TEXT("가위 공격 시작"));
}

void APCRSerinDollHandBaseCharacter::HandleScissors(float DeltaTime)
{
	switch (CurrentScissorsState)
	{
		case EScissorsState::Stay:
		{
			ScissorsCoolDownElapsedTime += DeltaTime;

			const FVector Direction = (CachedTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			const FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
			const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 7.f);
			SetActorRotation(NewRotation);

			if (ScissorsCoolDownElapsedTime >= ScissorsCoolDown)
			{
				ScissorsCoolDownElapsedTime = 0.f;
				CurrentScissorsState = EScissorsState::Attack;

				// TODO: 파라미터화 필요
				ChaseLocation = GetActorLocation() + (Direction * 750.f);
			}

			break;
		}
		case EScissorsState::Attack:
		{
			const FVector NewLocation = FMath::VInterpTo(GetActorLocation(), ChaseLocation, DeltaTime, 10.f);
			SetActorLocation(NewLocation);

			const float DistSquare = FVector::DistSquared(NewLocation, ChaseLocation);
			if (DistSquare <= FMath::Square(10.f))
			{
				++ScissorsAttackCount;
				UE_LOG(PCRLogSerinHandBaseCharacter, Log, TEXT("%d차 가위 공격 끝"), ScissorsAttackCount);
				if (ScissorsAttackCount >= ScissorsAttackMaxCount)
				{
					ScissorsAttackCount = 0;
					CurrentScissorsState = EScissorsState::Invalid;
					CurrentSerinState = ESerinState::BasicChase;
				}
				else
				{
					CurrentScissorsState = EScissorsState::Stay;
				}
			}

			break;
		}
		case EScissorsState::Invalid:
		{
			break;
		}
	}
}
