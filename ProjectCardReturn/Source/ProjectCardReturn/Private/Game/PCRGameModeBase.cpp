// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRGameModeBase.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Players/Erica/PCREricaPlayerController.h"
#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"
#include "Entities/MonsterGenerator/Base/PCRMonsterBaseGenerator.h"
#include "Entities/Monsters/MeleeSoldier/PCRMeleeSoldierCharacter.h"
#include "Game/PCRParameterDataAsset.h"

DEFINE_LOG_CATEGORY(PCRLogGameModeBase);

APCRGameModeBase::APCRGameModeBase()
{
	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}

	DefaultPawnClass = APCREricaCharacter::StaticClass();
	PlayerControllerClass = APCREricaPlayerController::StaticClass();
}

void APCRGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (ParameterDataAsset->bIsMonsterSpawn)
	{
		RETURN_IF_INVALID(IsValid(GetWorld()));
		MonsterGenerator1 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(1500.0, 0.0, 100.0), FRotator::ZeroRotator);
		MonsterGenerator2 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(-1500.0, 0.0, 100.0), FRotator::ZeroRotator);
		MonsterGenerator3 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(1500.0, -2000.0, 100.0), FRotator::ZeroRotator);
		MonsterGenerator4 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(-1500.0, -2000.0, 100.0), FRotator::ZeroRotator);
		MonsterGenerator5 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(1500.0, 2000.0, 100.0), FRotator::ZeroRotator);
		MonsterGenerator6 = GetWorld()->SpawnActor<APCRMonsterBaseGenerator>(FVector(-1500.0, 2000.0, 100.0), FRotator::ZeroRotator);

		// MonsterGenerator1->Start(APCRMeleeSoldierCharacter::StaticClass(), 3.f);
		// MonsterGenerator2->Start(APCRMeleeSoldierCharacter::StaticClass(), 3.f);
		MonsterGenerator3->Start(APCRMeleeSoldierCharacter::StaticClass(), 3.f);
		MonsterGenerator4->Start(APCRMeleeSoldierCharacter::StaticClass(), 3.f);
		MonsterGenerator5->Start(APCRRabbitCharacter::StaticClass(), 3.f);
		// MonsterGenerator6->Start(APCRRabbitCharacter::StaticClass(), 3.f);
	}
}

