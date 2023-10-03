// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRGameModeBase.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Players/Erica/PCREricaPlayerController.h"
#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"
#include "Entities/MonsterGenerator/Base/PCRMonsterBaseGenerator.h"

DEFINE_LOG_CATEGORY(PCRLogGameModeBase);

APCRGameModeBase::APCRGameModeBase()
{
	DefaultPawnClass = APCREricaCharacter::StaticClass();
	PlayerControllerClass = APCREricaPlayerController::StaticClass();
}

void APCRGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	RETURN_IF_INVALID(IsValid(GetWorld()));
	MonsterGenerator1 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(1500.0, 0.0, 0.0), FRotator::ZeroRotator);
	MonsterGenerator2 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(-1500.0, 0.0, 0.0), FRotator::ZeroRotator);
	MonsterGenerator3 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(1500.0, -2000.0, 0.0), FRotator::ZeroRotator);
	MonsterGenerator4 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(-1500.0, -2000.0, 0.0), FRotator::ZeroRotator);
	MonsterGenerator5 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(1500.0, 2000.0, 0.0), FRotator::ZeroRotator);
	MonsterGenerator6 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(-1500.0, 2000.0, 0.0), FRotator::ZeroRotator);
	
	MonsterGenerator1->Start(APCRRabbitCharacter::StaticClass(), 1.f);
	MonsterGenerator2->Start(APCRRabbitCharacter::StaticClass(), 1.f);
	MonsterGenerator3->Start(APCRRabbitCharacter::StaticClass(), 1.f);
	MonsterGenerator4->Start(APCRRabbitCharacter::StaticClass(), 1.f);
	MonsterGenerator5->Start(APCRRabbitCharacter::StaticClass(), 1.f);
	MonsterGenerator6->Start(APCRRabbitCharacter::StaticClass(), 1.f);
}
