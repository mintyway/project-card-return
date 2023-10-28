// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/Hand/Base/PCRSerinHandBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/Serin/PCRSerinCharacter.h"
#include "Entities/Boss/Serin/Base/PCRSerinPrimaryDataAsset.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(PCRLogSerinHandBaseCharacter);

APCRSerinHandBaseCharacter::APCRSerinHandBaseCharacter()
	: CurrentSerinState(ESerinState::Invalid)
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

void APCRSerinHandBaseCharacter::Move(const FVector& InLocation)
{
	MoveLocation = InLocation;
	CurrentSerinState = ESerinState::Move;
}

void APCRSerinHandBaseCharacter::BasicChase()
{
	CurrentSerinState = ESerinState::BasicChase;
}

void APCRSerinHandBaseCharacter::Chase()
{
	CurrentSerinState = ESerinState::Chase;
}

void APCRSerinHandBaseCharacter::Rock()
{
	OnChaseEnd.BindUObject(this, &APCRSerinHandBaseCharacter::RockCallback);
	Chase();
}

void APCRSerinHandBaseCharacter::HandleMove(float DeltaTime)
{
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), MoveLocation, DeltaTime, 500.f);
	SetActorLocation(NewLocation);
	const float DistSquare2D = FVector::DistSquared2D(NewLocation, MoveLocation);
	if (DistSquare2D <= FMath::Square(10.f))
	{
		CurrentSerinState = ESerinState::BasicChase;
	}
}

void APCRSerinHandBaseCharacter::HandleBasicChase(float DeltaTime)
{
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, 500.f);
	SetActorLocation(NewLocation);
}

void APCRSerinHandBaseCharacter::HandleChase(float DeltaTime)
{
	// TODO: 플레이어 위 높이 파라미터화 필요
	TargetLocation = CachedTarget->GetActorLocation() + FVector(0.0, 0.0, 300.0);
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, 1000.f);
	SetActorLocation(NewLocation);

	const float DistSquare2D = FVector::DistSquared2D(NewLocation, CachedTarget->GetActorLocation());
	if (DistSquare2D <= FMath::Square(10.f))
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
	UE_LOG(LogTemp, Warning, TEXT("주먹 공격 시작"));
	TargetLocation = GetActorLocation() - FVector(0.0, 0.0, 300.0);
}

void APCRSerinHandBaseCharacter::HandleRock(float DeltaTime)
{
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, 3000.f);
	SetActorLocation(NewLocation);
	const float DistSquare = FVector::DistSquared(NewLocation, TargetLocation);
	if (DistSquare <= FMath::Square(10.f))
	{
		UE_LOG(LogTemp, Warning, TEXT("주먹 공격 끝"));
		CurrentSerinState = ESerinState::BasicChase;
	}
}
