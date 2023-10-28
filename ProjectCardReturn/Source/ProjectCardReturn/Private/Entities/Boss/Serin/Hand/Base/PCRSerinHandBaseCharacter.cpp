// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/Hand/Base/PCRSerinHandBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/Serin/PCRSerinCharacter.h"
#include "Entities/Boss/Serin/Base/PCRSerinPrimaryDataAsset.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		case ESerinState::Rock:
		{
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

void APCRSerinHandBaseCharacter::Chase()
{
	CurrentSerinState = ESerinState::Chase;
}

void APCRSerinHandBaseCharacter::HandleMove(float DeltaTime)
{
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), MoveLocation, DeltaTime, 500.f);
	SetActorLocation(NewLocation);
	const float DistSquare2D = FVector::DistSquared2D(NewLocation, CachedTarget->GetActorLocation());
	if (DistSquare2D <= FMath::Square(10.f))
	{
		CurrentSerinState = ESerinState::Chase;
	}
}

void APCRSerinHandBaseCharacter::HandleChase(float DeltaTime)
{
	const FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaTime, 500.f);
	SetActorLocation(NewLocation);
}
