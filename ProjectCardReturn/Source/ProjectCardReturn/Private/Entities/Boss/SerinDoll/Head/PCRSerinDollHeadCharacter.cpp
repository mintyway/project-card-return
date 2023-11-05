// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollAIController.h"
#include "Entities/Boss/SerinDoll/Base/PCRSerinDollPrimaryDataAsset.h"

#include "Components/CapsuleComponent.h"
#include "Entities/Boss/SerinDoll/Hand/PCRSerinDollHandCharacter.h"
#include "Entities/Players/Erica/PCREricaPlayerController.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

const float APCRSerinDollHeadCharacter::ContactDistance = 10.f;
const float APCRSerinDollHeadCharacter::FloatingHandHeight = 500.f;
const float APCRSerinDollHeadCharacter::BasicChaseYDistance = 700.f;

APCRSerinDollHeadCharacter::APCRSerinDollHeadCharacter()
	: bIsAlive(true)
{
	PrimaryActorTick.bCanEverTick = true;

	// 파라미터화 필요
	MaxHP = 1000.f;
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
		GetMesh()->SetSkeletalMesh(SerinDollDataAsset->HeadMesh);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->GravityScale = 0.f;
	}
}

void APCRSerinDollHeadCharacter::PostInitializeComponents()
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

void APCRSerinDollHeadCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundErica;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APCREricaCharacter::StaticClass(), TEXT("Erica"), FoundErica);

	CachedErica = Cast<APCREricaCharacter>(FoundErica[0]);
	check(CachedErica);

	LeftHand->Idle(CachedErica);
	RightHand->Idle(CachedErica);

	// TODO: 테스트용 코드
	FTimerHandle TestTimerHandle1;
	TimerHandles.Add(TestTimerHandle1);
	GetWorldTimerManager().SetTimer(TestTimerHandle1, FTimerDelegate::CreateLambda([this]() -> void
	{
		LeftPaperAttack();

	}), 10.f, true, 0.f);

	FTimerHandle TestTimerHandle2;
	TimerHandles.Add(TestTimerHandle2);
	GetWorldTimerManager().SetTimer(TestTimerHandle2, FTimerDelegate::CreateLambda([this]() -> void
	{
		RightPaperAttack();
	}), 10.f, true, 5.f);

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

void APCRSerinDollHeadCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float APCRSerinDollHeadCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ChangeHP(-Damage);
	return Damage;
}

void APCRSerinDollHeadCharacter::LeftRockAttack()
{
	if (CheckIsAttacking(LeftHand))
	{
		return;
	}
	
	LeftHand->RockAttack(CachedErica);
}

void APCRSerinDollHeadCharacter::RightRockAttack()
{
	if (CheckIsAttacking(RightHand))
	{
		return;
	}
	
	RightHand->RockAttack(CachedErica);
}

void APCRSerinDollHeadCharacter::LeftPaperAttack()
{
	if (CheckIsAttacking(LeftHand))
	{
		return;
	}
	
	LeftHand->PaperAttack();
}

void APCRSerinDollHeadCharacter::RightPaperAttack()
{
	if (CheckIsAttacking(RightHand))
	{
		return;
	}
	
	RightHand->PaperAttack();
}

void APCRSerinDollHeadCharacter::LeftScissorsAttack()
{
	if (CheckIsAttacking(LeftHand))
	{
		return;
	}
	
	LeftHand->ScissorsAttack(CachedErica);
}

void APCRSerinDollHeadCharacter::RightScissorsAttack()
{
	if (CheckIsAttacking(RightHand))
	{
		return;
	}
	
	RightHand->ScissorsAttack(CachedErica);
}

void APCRSerinDollHeadCharacter::SpawnHands()
{
	LeftHandSpawn();
	RightHandSpawn();

#if WITH_EDITOR
	LeftHand->SetActorLabel(LeftHand->GetName());
	RightHand->SetActorLabel(RightHand->GetName());
	LeftHand->SetFolderPath(TEXT("Serin"));
	RightHand->SetFolderPath(TEXT("Serin"));
#endif
}

void APCRSerinDollHeadCharacter::LeftHandSpawn()
{
	const FVector LeftHandSpawnLocation = GetActorLocation() + FVector(-1000.0, 1000.0, 500.0);
	const FRotator LeftHandSpawnRotation = GetActorRotation();

	FActorSpawnParameters LeftHandSpawnParameters;
	const FName LeftHandName = TEXT("LeftHand");
	LeftHandSpawnParameters.Name = LeftHandName;
	LeftHand = GetWorld()->SpawnActor<APCRSerinDollHandCharacter>(APCRSerinDollHandCharacter::StaticClass(), LeftHandSpawnLocation, LeftHandSpawnRotation, LeftHandSpawnParameters);
	
	LeftHand->Init(this, -GetActorRightVector());
}

void APCRSerinDollHeadCharacter::RightHandSpawn()
{
	const FVector RightHandSpawnLocation = GetActorLocation() + FVector(-1000.0, -1000.0, 500.0);
	const FRotator RightHandSpawnRotation = GetActorRotation();

	FActorSpawnParameters RightHandSpawnParameters;
	const FName RightHandName = TEXT("RightHand");
	RightHandSpawnParameters.Name = RightHandName;
	RightHand = GetWorld()->SpawnActor<APCRSerinDollHandCharacter>(APCRSerinDollHandCharacter::StaticClass(), RightHandSpawnLocation, RightHandSpawnRotation, RightHandSpawnParameters);
	
	RightHand->Init(this, GetActorRightVector());

	// 좌우 반전 코드
	const FVector NewScale = RightHand->GetMesh()->GetRelativeScale3D() * FVector(-1.0, 1.0, 1.0);
	RightHand->GetMesh()->SetRelativeScale3D(NewScale);
}

void APCRSerinDollHeadCharacter::ChangeHP(float Amount)
{
	CurrentHP += Amount;
	HandleChangeHP();
}

void APCRSerinDollHeadCharacter::HandleChangeHP()
{
	if (CurrentHP <= 0.f)
	{
		CurrentHP = 0.f;
		HandleDead();
	}

	OnChangeHP.Broadcast(MaxHP, CurrentHP);
}

void APCRSerinDollHeadCharacter::HandleDead()
{
	bIsAlive = false;

	FTimerHandle DelayedDestroyHandle;
	FTimerDelegate DelayedDestroyDelegate;
	DelayedDestroyDelegate.BindUObject(this, &APCRSerinDollHeadCharacter::DelayedDestroy);
	GetWorldTimerManager().SetTimer(DelayedDestroyHandle, DelayedDestroyDelegate, 1.f, false);
}

void APCRSerinDollHeadCharacter::DelayedDestroy()
{
	for (auto& TimerHandle : TimerHandles)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}

	RightHand->Destroy();
	LeftHand->Destroy();
	Destroy();
}

bool APCRSerinDollHeadCharacter::CheckIsAttacking(APCRSerinDollHandCharacter* InSerinDollHand)
{
	return InSerinDollHand->GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr);
}

float APCRSerinDollHeadCharacter::GetLiftHeight()
{
	const float LiftHeight = CachedLift->GetActorLocation().Z;
	return LiftHeight;
}

float APCRSerinDollHeadCharacter::GetHandWorldHeight()
{
	const float HandWorldHeight = GetLiftHeight() + FloatingHandHeight;
	return HandWorldHeight;
}
