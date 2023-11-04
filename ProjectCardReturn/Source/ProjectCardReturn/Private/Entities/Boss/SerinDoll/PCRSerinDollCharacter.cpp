// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll//PCRSerinDollCharacter.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Boss/SerinDoll/PCRSerinDollAIController.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollLeftHandCharacter.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollRightHandCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Players/Erica/PCREricaPlayerController.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

const float APCRSerinDollCharacter::ContactDistance = 10.f;
const float APCRSerinDollCharacter::FloatingHandHeight = 500.f;
const float APCRSerinDollCharacter::BasicChaseYDistance = 700.f;

APCRSerinDollCharacter::APCRSerinDollCharacter()
	: MaxHP(1000.f), CurrentHP(0.f),
	  bIsAlive(true)
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentHP = MaxHP;

	AIControllerClass = APCRSerinDollAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->InitCapsuleSize(400.f, 877.f);
		GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel9);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	}
	
	if (GetMesh() && SerinDollDataAsset)
	{
		GetMesh()->SetupAttachment(GetCapsuleComponent());
		GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
		GetMesh()->SetSkeletalMesh(SerinDollDataAsset->SerinDollMesh);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = 0.f;
	}
}

void APCRSerinDollCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

#if WITH_EDITOR
	if (!GetWorld()->IsPlayInEditor())
	{
		return;
	}
	
	SetActorLabel(TEXT("Serin"));
	SetFolderPath(TEXT("Serin"));
#endif

	TArray<AActor*> Lifts;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APCRLiftActor::StaticClass(), TEXT("Lift"), Lifts);
	CachedLift = Cast<APCRLiftActor>(Lifts[0]);
	check(CachedLift);

	SpawnHands();
}

void APCRSerinDollCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundErica;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APCREricaCharacter::StaticClass(), TEXT("Erica"), FoundErica);

	CachedErica = Cast<APCREricaCharacter>(FoundErica[0]);
	check(CachedErica);

	LeftHand->SetTarget(CachedErica);
	// RightHand->SetTarget(CachedErica);

	// LeftHand->BasicChase();
	// RightHand->BasicChase();
	
	// TODO: 테스트용 코드
	FTimerHandle TestTimerHandle1;
	TimerHandles.Add(TestTimerHandle1);
	GetWorldTimerManager().SetTimer(TestTimerHandle1, FTimerDelegate::CreateLambda([this]() -> void
	{
		LeftHand->GetMesh()->GetAnimInstance()->Montage_Play(SerinDollDataAsset->LeftHandScissorsAnimMontage);
		// LeftHand->ScissorsAttack();
	}), 5.f, true, 1.f);

	// FTimerHandle TestTimerHandle2;
	// TimerHandles.Add(TestTimerHandle2);
	// GetWorldTimerManager().SetTimer(TestTimerHandle2, FTimerDelegate::CreateLambda([this]() -> void
	// {
	// 	RightHand->ScissorsAttack();
	// }), 30.f, true, 5.f);
	//
	// FTimerHandle TestTimerHandle3;
	// TimerHandles.Add(TestTimerHandle3);
	// GetWorldTimerManager().SetTimer(TestTimerHandle3, FTimerDelegate::CreateLambda([this]() -> void
	// {
	// 	LeftHand->PaperAttack();
	// }), 30.f, true, 10.f);
	//
	// FTimerHandle TestTimerHandle4;
	// TimerHandles.Add(TestTimerHandle4);
	// GetWorldTimerManager().SetTimer(TestTimerHandle4, FTimerDelegate::CreateLambda([this]() -> void
	// {
	// 	RightHand->RockAttack();
	// }), 30.f, true, 15.f);
	//
	// FTimerHandle TestTimerHandle5;
	// TimerHandles.Add(TestTimerHandle5);
	// GetWorldTimerManager().SetTimer(TestTimerHandle5, FTimerDelegate::CreateLambda([this]() -> void
	// {
	// 	LeftHand->ScissorsAttack();
	// }), 30.f, true, 20.f);
	//
	// FTimerHandle TestTimerHandle6;
	// TimerHandles.Add(TestTimerHandle6);
	// GetWorldTimerManager().SetTimer(TestTimerHandle6, FTimerDelegate::CreateLambda([this]() -> void
	// {
	// 	RightHand->PaperAttack();
	// }), 30.f, true, 25.f);
	
	APCREricaPlayerController* EricaPlayerController = Cast<APCREricaPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	check(EricaPlayerController);
	EricaPlayerController->BindSerinUI(this);
}

void APCRSerinDollCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float APCRSerinDollCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ChangeHP(-Damage);
	return Damage;
}

void APCRSerinDollCharacter::SpawnHands()
{
	FVector LeftHandSpawnLocation = GetActorLocation() + FVector(-1000.0, 1000.0, 0.0);
	LeftHandSpawnLocation.Z = 1500.0;
	const FRotator LeftHandSpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters LeftHandSpawnParameters;
	const FName LeftHandName = TEXT("LeftHand");
	LeftHandSpawnParameters.Name = LeftHandName;
	LeftHand = GetWorld()->SpawnActor<APCRSerinDollLeftHandCharacter>(APCRSerinDollLeftHandCharacter::StaticClass(), LeftHandSpawnLocation, LeftHandSpawnRotation, LeftHandSpawnParameters);
	LeftHand->SetSerinDollCharacter(this);

	// FVector RightHandSpawnLocation = GetActorLocation() + FVector(-300.0, -1000.0, 0.0);
	// RightHandSpawnLocation.Z = 1000.0;
	// const FRotator RightHandSpawnRotation = FRotator::ZeroRotator;
	// FActorSpawnParameters RightHandSpawnParameters;
	// const FName RightHandName = TEXT("RightHand");
	// RightHandSpawnParameters.Name = RightHandName;
	// RightHand = GetWorld()->SpawnActor<APCRSerinDollRightHandCharacter>(APCRSerinDollRightHandCharacter::StaticClass(), RightHandSpawnLocation, RightHandSpawnRotation, RightHandSpawnParameters);
	// RightHand->SetSerinDollCharacter(this);

#if WITH_EDITOR
	LeftHand->SetActorLabel(LeftHand->GetName());
	// RightHand->SetActorLabel(RightHand->GetName());
	LeftHand->SetFolderPath(TEXT("Serin"));
	// RightHand->SetFolderPath(TEXT("Serin"));
#endif
}

void APCRSerinDollCharacter::ChangeHP(float Amount)
{
	CurrentHP += Amount;
	HandleChangeHP();
}

void APCRSerinDollCharacter::HandleChangeHP()
{
	if (CurrentHP <= 0.f)
	{
		CurrentHP = 0.f;
		HandleDead();
	}

	OnChangeHP.Broadcast(MaxHP, CurrentHP);
}

void APCRSerinDollCharacter::HandleDead()
{
	bIsAlive = false;

	FTimerHandle DelayedDestroyHandle;
	FTimerDelegate DelayedDestroyDelegate;
	DelayedDestroyDelegate.BindUObject(this, &APCRSerinDollCharacter::DelayedDestroy);
	GetWorldTimerManager().SetTimer(DelayedDestroyHandle, DelayedDestroyDelegate, 1.f, false);
}

void APCRSerinDollCharacter::DelayedDestroy()
{
	for (auto& TimerHandle : TimerHandles)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
	
	RightHand->Destroy();
	LeftHand->Destroy();
	Destroy();
}

float APCRSerinDollCharacter::GetLiftHeight()
{
	const float LiftHeight = CachedLift->GetActorLocation().Z;
	return LiftHeight;
}

float APCRSerinDollCharacter::GetHandWorldHeight()
{
	const float HandWorldHeight = GetLiftHeight() + FloatingHandHeight;
	return HandWorldHeight;
}

void APCRSerinDollCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
