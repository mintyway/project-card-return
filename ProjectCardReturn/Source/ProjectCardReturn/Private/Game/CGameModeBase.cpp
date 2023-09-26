// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CGameModeBase.h"

#include "Entities/Players/Erica/CEricaCharacter.h"
#include "Entities/Players/Erica/CEricaPlayerController.h"
#include "Entities/Monsters/Rabbit/CRabbitCharacter.h"
#include "Entities/MonsterGenerator/Base/CMonsterBaseGenerator.h"

ACGameModeBase::ACGameModeBase()
{
	DefaultPawnClass = ACEricaCharacter::StaticClass();
	PlayerControllerClass = ACEricaPlayerController::StaticClass();
}

void ACGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	RETURN_IF_INVALID(IsValid(GetWorld()));
	MonsterGenerator1 = GetWorld()->SpawnActor<ACMonsterBaseGenerator>(FVector(2000.0, 0.0, 0.0), FRotator::ZeroRotator);
	MonsterGenerator2 = GetWorld()->SpawnActor<ACMonsterBaseGenerator>(FVector(-2000.0, 0.0, 0.0), FRotator::ZeroRotator);
	MonsterGenerator3 = GetWorld()->SpawnActor<ACMonsterBaseGenerator>(FVector(0.0, 2000.0, 0.0), FRotator::ZeroRotator);
	MonsterGenerator4 = GetWorld()->SpawnActor<ACMonsterBaseGenerator>(FVector(0.0, -2000.0, 0.0), FRotator::ZeroRotator);

	MonsterGenerator1->Start(ACRabbitCharacter::StaticClass(), 1.f);
	MonsterGenerator2->Start(ACRabbitCharacter::StaticClass(), 1.f);
	MonsterGenerator3->Start(ACRabbitCharacter::StaticClass(), 1.f);
	MonsterGenerator4->Start(ACRabbitCharacter::StaticClass(), 1.f);
}
