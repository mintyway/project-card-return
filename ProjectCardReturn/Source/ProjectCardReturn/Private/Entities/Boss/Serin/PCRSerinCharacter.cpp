// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/Serin/PCRSerinCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/Serin/PCRSerinAIController.h"
#include "Entities/Boss/Serin/Base/PCRSerinPrimaryDataAsset.h"
#include "Entities/Boss/Serin/Hand/PCRSerinLeftHandCharacter.h"
#include "Entities/Boss/Serin/Hand/PCRSerinRightHandCharacter.h"
#include "Kismet/GameplayStatics.h"

// 테스트
#include "EngineUtils.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

APCRSerinCharacter::APCRSerinCharacter()
	: LeftHandChaseDistance(0.0, 700.0, 300.0), RightHandChaseDistance(0.0, -700.0, 300.0)
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

	SetFolderPath(TEXT("Serin"));

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
	RightHand->Chase();
}

void APCRSerinCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APCRSerinCharacter::SpawnHands()
{
	FVector LeftHandSpawnLocation = GetActorLocation() + FVector(-300.0, 300.0, 0.0);
	LeftHandSpawnLocation.Z = 1000.0;
	const FRotator LeftHandSpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters LeftHandSpawnParameters;
	LeftHandSpawnParameters.Owner = this;
	const FName LeftHandName = TEXT("LeftHand");
	LeftHandSpawnParameters.Name = LeftHandName;
	LeftHand = GetWorld()->SpawnActor<APCRSerinLeftHandCharacter>(APCRSerinLeftHandCharacter::StaticClass(), LeftHandSpawnLocation, LeftHandSpawnRotation, LeftHandSpawnParameters);
	LeftHand->SetActorLabel(LeftHand->GetName());
	LeftHand->SetFolderPath(TEXT("Serin"));

	FVector RightHandSpawnLocation = GetActorLocation() + FVector(-300.0, -300.0, 0.0);
	RightHandSpawnLocation.Z = 1000.0; 
	const FRotator RightHandSpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters RightHandSpawnParameters;
	RightHandSpawnParameters.Owner = this;
	const FName RightHandName = TEXT("RightHand");
	RightHandSpawnParameters.Name = RightHandName;
	RightHand = GetWorld()->SpawnActor<APCRSerinRightHandCharacter>(APCRSerinRightHandCharacter::StaticClass(), RightHandSpawnLocation, RightHandSpawnRotation, RightHandSpawnParameters);
	RightHand->SetActorLabel(RightHand->GetName());
	RightHand->SetFolderPath(TEXT("Serin"));
}

void APCRSerinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
