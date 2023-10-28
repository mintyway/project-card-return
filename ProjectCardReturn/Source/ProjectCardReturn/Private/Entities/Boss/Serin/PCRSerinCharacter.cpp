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
	: LeftHandBasicChaseDistance(0.0, 700.0, 300.0), RightHandBasicChaseDistance(0.0, -700.0, 300.0)
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
	
	// TODO: 테스트용 코드
	FTimerHandle TestTimerHandle1;
	GetWorldTimerManager().SetTimer(TestTimerHandle1, FTimerDelegate::CreateLambda([this]() -> void
	{
		LeftHand->Rock();
	}), 5.f, true, 0.f);

	FTimerHandle TestTimerHandle2;
	GetWorldTimerManager().SetTimer(TestTimerHandle2, FTimerDelegate::CreateLambda([this]() -> void
	{
		RightHand->Rock();
	}), 5.f, true, 2.5f);
	
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
