// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll//PCRSerinCharacter.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Boss/SerinDoll/PCRSerinAIController.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinLeftHandCharacter.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinRightHandCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

const float APCRSerinCharacter::ContactDistance = 10.f;
const float APCRSerinCharacter::DefaultFloatingHeight = 1200.f;
const float APCRSerinCharacter::LeftHandBasicChaseYDistance = 700.f;
const float APCRSerinCharacter::RightHandBasicChaseYDistance = -700.f;

APCRSerinCharacter::APCRSerinCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = APCRSerinAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DummyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyMeshComponent"));
	if (DummyMeshComponent && SerinDataAsset)
	{
		DummyMeshComponent->SetupAttachment(GetCapsuleComponent());
		DummyMeshComponent->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
		DummyMeshComponent->SetRelativeScale3D(FVector(4.0, 4.0, 4.0));
		DummyMeshComponent->SetStaticMesh(SerinDataAsset->SerinDummyMesh);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = 0.f;
	}
}

void APCRSerinCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

#if WITH_EDITOR
	SetActorLabel(TEXT("Serin"));
	SetFolderPath(TEXT("Serin"));
#endif

	TArray<AActor*> Lifts;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APCRLiftActor::StaticClass(), TEXT("Lift"), Lifts);
	CachedLift = Cast<APCRLiftActor>(Lifts[0]);
	check(CachedLift);
	
	SpawnHands();
}

void APCRSerinCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundErica;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APCREricaCharacter::StaticClass(), TEXT("Erica"), FoundErica);

	CachedErica = Cast<APCREricaCharacter>(FoundErica[0]);
	check(CachedErica);

	LeftHand->SetTarget(CachedErica);
	RightHand->SetTarget(CachedErica);

	const FVector NewLocation = CachedErica->GetActorLocation() + FVector(0.0, 0.0, 300.0);
	LeftHand->Move(NewLocation);

	// TODO: 테스트용 코드
	FTimerHandle TestTimerHandle1;
	GetWorldTimerManager().SetTimer(TestTimerHandle1, FTimerDelegate::CreateLambda([this]() -> void
	{
		LeftHand->RockAttack();
	}), 10.f, true, 0.f);

	FTimerHandle TestTimerHandle2;
	GetWorldTimerManager().SetTimer(TestTimerHandle2, FTimerDelegate::CreateLambda([this]() -> void
	{
		RightHand->RockAttack();
	}), 10.f, true, 5.f);

	RightHand->Chase();
}

void APCRSerinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCRSerinCharacter::SpawnHands()
{
	FVector LeftHandSpawnLocation = GetActorLocation() + FVector(-300.0, 1000.0, 0.0);
	LeftHandSpawnLocation.Z = 1000.0;
	const FRotator LeftHandSpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters LeftHandSpawnParameters;
	LeftHandSpawnParameters.Owner = this;
	const FName LeftHandName = TEXT("LeftHand");
	LeftHandSpawnParameters.Name = LeftHandName;
	LeftHand = GetWorld()->SpawnActor<APCRSerinLeftHandCharacter>(APCRSerinLeftHandCharacter::StaticClass(), LeftHandSpawnLocation, LeftHandSpawnRotation, LeftHandSpawnParameters);

	FVector RightHandSpawnLocation = GetActorLocation() + FVector(-300.0, -1000.0, 0.0);
	RightHandSpawnLocation.Z = 1000.0;
	const FRotator RightHandSpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters RightHandSpawnParameters;
	RightHandSpawnParameters.Owner = this;
	const FName RightHandName = TEXT("RightHand");
	RightHandSpawnParameters.Name = RightHandName;
	RightHand = GetWorld()->SpawnActor<APCRSerinRightHandCharacter>(APCRSerinRightHandCharacter::StaticClass(), RightHandSpawnLocation, RightHandSpawnRotation, RightHandSpawnParameters);

#if WITH_EDITOR
	LeftHand->SetActorLabel(LeftHand->GetName());
	RightHand->SetActorLabel(RightHand->GetName());
	LeftHand->SetFolderPath(TEXT("Serin"));
	RightHand->SetFolderPath(TEXT("Serin"));
#endif
}

float APCRSerinCharacter::GetLiftHeight()
{
	const float LiftHeight = CachedLift->GetActorLocation().Z;
	return LiftHeight;
}

void APCRSerinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
