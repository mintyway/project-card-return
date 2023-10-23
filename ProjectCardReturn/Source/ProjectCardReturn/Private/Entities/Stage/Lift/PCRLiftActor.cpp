// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Stage/Lift/PCRLiftActor.h"

#include "Entities/Stage/Base/PCRStagePrimaryDataAsset.h"

// Sets default values
APCRLiftActor::APCRLiftActor() : MaxLiftHeight(350.f), LiftingTime(3.0), ElapsedTime(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		RootComponent = SceneComponent;
	}
	
	LiftMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LiftMeshComponent"));
	if (LiftMeshComponent)
	{
		LiftMeshComponent->SetupAttachment(SceneComponent);
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

	HandleLeftUp(DeltaTime);
}

void APCRLiftActor::LiftUp()
{
	SetActorTickEnabled(true);
	StartLiftLocation = GetActorLocation();
	EndLiftLocation = FVector(StartLiftLocation.X, StartLiftLocation.Y, MaxLiftHeight);
}

void APCRLiftActor::HandleLeftUp(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	const float alpha = FMath::Clamp(ElapsedTime / LiftingTime, 0.f, 1.f);
	const FVector NewLocation = FMath::Lerp(StartLiftLocation, EndLiftLocation, alpha);
	SetActorLocation(NewLocation);
	
	if (alpha >= 1.f)
	{
		SetActorTickEnabled(false);
	}
}
