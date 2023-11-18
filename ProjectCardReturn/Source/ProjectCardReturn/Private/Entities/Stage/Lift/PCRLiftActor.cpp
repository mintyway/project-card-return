// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Stage/Lift/PCRLiftActor.h"

#include "Components/BoxComponent.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Stage/Base/PCRStagePrimaryDataAsset.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(PCRLogRiftActor);

// Sets default values
APCRLiftActor::APCRLiftActor()
	: State(EState::LiftUp), MaxLiftHeight(350.f), LiftingTime(3.0), ElapsedTime(0.f), bIsOverlappedPattern1(false)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		SetRootComponent(SceneComponent);
	}

	LiftMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LiftMeshComponent"));
	if (LiftMeshComponent)
	{
		LiftMeshComponent->SetupAttachment(GetRootComponent());
	}

	Pattern1SuccessBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Pattern1SuccessBoxComponent"));
	if (Pattern1SuccessBoxComponent)
	{
		Pattern1SuccessBoxComponent->SetupAttachment(GetRootComponent());
		Pattern1SuccessBoxComponent->SetRelativeLocation(FVector(750.0, 0.0, 0.0));
		Pattern1SuccessBoxComponent->InitBoxExtent(FVector(300.0, 300.0, 500.0));
	}
}

void APCRLiftActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

void APCRLiftActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
		case EState::LiftUp:
		{
			UpdateLiftUp(DeltaTime);
			break;
		}
		case EState::SerinPattern1:
		{
			UpdatePattern1OverlapCheck();
			break;
		}
	}
}

void APCRLiftActor::LiftUp()
{
	SetActorTickEnabled(true);
	StartLiftLocation = GetActorLocation();
	EndLiftLocation = FVector(StartLiftLocation.X, StartLiftLocation.Y, MaxLiftHeight);
	State = EState::LiftUp;

	OnLiftUpStart.Broadcast();
}

void APCRLiftActor::SerinPattern1Start()
{
	SIMPLE_LOG;
	TArray<AActor*> EricaArray;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(),
												 APCREricaCharacter::StaticClass(),
												 TEXT("Erica"), EricaArray);
	CachedErica = Cast<APCREricaCharacter>(EricaArray[0]);
	check(CachedErica);
	
	SetActorTickEnabled(true);
	
	State = EState::SerinPattern1;
}

void APCRLiftActor::SerinPattern1End()
{
	UE_LOG(PCRLogRiftActor, Log, TEXT("SerinPattern1End"));
	SetActorTickEnabled(false);
}

bool APCRLiftActor::IsOverlappedPattern1() const
{
	return bIsOverlappedPattern1;
}

void APCRLiftActor::UpdateLiftUp(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	const float alpha = FMath::Clamp(ElapsedTime / LiftingTime, 0.f, 1.f);
	const FVector NewLocation = FMath::Lerp(StartLiftLocation, EndLiftLocation, alpha);
	SetActorLocation(NewLocation);

	if (alpha >= 1.f)
	{
		SetActorTickEnabled(false);
		OnLiftUpEnd.Broadcast();
	}
}

void APCRLiftActor::UpdatePattern1OverlapCheck()
{
	if (Pattern1SuccessBoxComponent->IsOverlappingActor(CachedErica))
	{
		bIsOverlappedPattern1 = true;
	}
	else
	{
		bIsOverlappedPattern1 = false;
	}
}
