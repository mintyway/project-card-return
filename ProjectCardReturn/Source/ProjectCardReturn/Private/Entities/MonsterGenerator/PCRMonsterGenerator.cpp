// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/MonsterGenerator/PCRMonsterGenerator.h"

#include "Engine/DamageEvents.h"
#include "Game/PCRParameterDataAsset.h"
#include "Entities/Monsters/Base/PCRMonsterBaseCharacter.h"

DEFINE_LOG_CATEGORY(PCRLogMonsterGenerator);

APCRMonsterGenerator::APCRMonsterGenerator() : SpawnRangeRadius(300.f), MonsterKillCount(0)
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}
	
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	if (RootSceneComponent)
	{
		RootComponent = RootSceneComponent;
	}
}

void APCRMonsterGenerator::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(ParameterDataAsset);
}

void APCRMonsterGenerator::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(PCRLogMonsterGenerator, Log, TEXT("%s이(가) 성공적으로 생성되었습니다."), *GetName());
}

void APCRMonsterGenerator::SpawnMonster(UClass* MonsterClass)
{
	if (!ParameterDataAsset->bIsMonsterSpawn)
	{
		return;
	}
	
	const FVector2D RandomLocation2D = FMath::RandPointInCircle(SpawnRangeRadius);
	const FVector RandomLocation = FVector(RandomLocation2D.X, RandomLocation2D.Y, 0.0);

	const FVector SpawnLocation = GetActorLocation() + RandomLocation;

	UE_LOG(PCRLogMonsterGenerator, Log, TEXT("몬스터 스폰 위치: %s"), *SpawnLocation.ToString());

	if (APCRMonsterBaseCharacter* NewMonster = GetWorld()->SpawnActor<APCRMonsterBaseCharacter>(MonsterClass, SpawnLocation, FRotator::ZeroRotator))
	{
		SpawnMonsters.Add(NewMonster);
		NewMonster->OnDead.AddUObject(this, &APCRMonsterGenerator::RemoveDeadMonster);
	}
}

void APCRMonsterGenerator::KillSpawnedMonsters()
{
	if (SpawnMonsters.IsEmpty())
	{
		return;
	}

	TArray<APCRMonsterBaseCharacter*> KillMonsters;
	for (const auto& SpawnMonster : SpawnMonsters)
	{
		if (SpawnMonster)
		{
			KillMonsters.Add(SpawnMonster);
		}
	}

	for (const auto& KillMonster : KillMonsters)
	{
		KillMonster->ChangeHP(-99999);
	}
}

void APCRMonsterGenerator::RemoveDeadMonster(APCRMonsterBaseCharacter* MonsterBaseCharacter)
{
	if (MonsterBaseCharacter)
	{
		SpawnMonsters.Remove(MonsterBaseCharacter);
		++MonsterKillCount;
	}

	if (OnSpawnedMonsterDead.IsBound())
	{
		OnSpawnedMonsterDead.Execute();
	}
}
