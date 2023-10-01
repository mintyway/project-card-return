// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/MonsterGenerator/Base/PCRMonsterBaseGenerator.h"

#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

APCRMonsterBaseGenerator::APCRMonsterBaseGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentGenerateMonster = nullptr;
	GenerateInterval = 0.f;
	SpawnRangeRadius = 300.f;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	if (RootSceneComponent)
	{
		RootComponent = RootSceneComponent;
	}
}

void APCRMonsterBaseGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void APCRMonsterBaseGenerator::Start(UClass* MonsterClass, float Interval)
{
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateLambda([this, MonsterClass]() -> void
	{
		SpawnMonster(MonsterClass);
	}), Interval, true);
}

void APCRMonsterBaseGenerator::Stop()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

void APCRMonsterBaseGenerator::SpawnMonster(UClass* MonsterClass) const
{
	const FVector2D RandomLocation2D = FMath::RandPointInCircle(SpawnRangeRadius);
	const FVector RandomLocation = FVector(RandomLocation2D.X, RandomLocation2D.Y, 0.0);
	const FVector SpawnLocation = GetActorLocation() + RandomLocation;

	UE_LOG(MonsterGeneratorLog, Warning, TEXT("몬스터 스폰 위치: %s"), *SpawnLocation.ToString());
	GetWorld()->SpawnActor<APCRMonsterBaseCharacter>(MonsterClass, SpawnLocation, FRotator::ZeroRotator);
}
