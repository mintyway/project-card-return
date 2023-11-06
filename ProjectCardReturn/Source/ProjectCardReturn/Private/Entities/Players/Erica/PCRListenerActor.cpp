// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Players/Erica/PCRListenerActor.h"

APCRListenerActor::APCRListenerActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if (SceneComponent)
	{
		RootComponent = SceneComponent;
	}
}

void APCRListenerActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetActorTickEnabled(false);
}

void APCRListenerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(Target->GetActorLocation());
}

void APCRListenerActor::Init(AActor* NewActor)
{
	Target = NewActor;
	check(Target);
	SetActorTickEnabled(true);
}
