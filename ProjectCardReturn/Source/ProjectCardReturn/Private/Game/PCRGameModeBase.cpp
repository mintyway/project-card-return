// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRGameModeBase.h"

#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
#include "Entities/Stage/Base/PCRStagePrimaryDataAsset.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Players/Erica/PCREricaPlayerController.h"
#include "Entities/MonsterGenerator/PCRMonsterGenerator.h"
#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"
#include "Entities/Monsters/Rabbit/EliteRabbit/PCREliteRabbitCharacter.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierCharacter.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/EliteMeleeSoldier/PCREliteMeleeSoldierCharacter.h"
#include "Entities/Monsters/Soldier/RangedSoldier/PCRRangedSoldierCharacter.h"
#include "Entities/Monsters/Soldier/RangedSoldier/EliteRangedSoldier/PCREliteRangedSoldierCharacter.h"
#include "Game/PCRGameInstance.h"
#include "Game/PCRParameterDataAsset.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(PCRLogGameModeBase);

APCRGameModeBase::APCRGameModeBase()
	: Phase(1), Stage1DefaultMonsterSpawnCount(0), Stage1EliteMonsterSpawnCount(0), Stage1TotalMonsterKillCount(0), Stage1TargetKillCount(100), CurrentStageState(EStageState::Stage)
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = APCREricaCharacter::StaticClass();
	PlayerControllerClass = APCREricaPlayerController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UPCRStagePrimaryDataAsset> DA_Stage(TEXT("/Script/ProjectCardReturn.PCRStagePrimaryDataAsset'/Game/DataAssets/DA_Stage.DA_Stage'"));
	if (DA_Stage.Succeeded())
	{
		StageDataAsset = DA_Stage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRParameterDataAsset> DA_Parameter(TEXT("/Script/ProjectCardReturn.PCRParameterDataAsset'/Game/DataAssets/DA_Parameter.DA_Parameter'"));
	if (DA_Parameter.Succeeded())
	{
		ParameterDataAsset = DA_Parameter.Object;
	}

	static ConstructorHelpers::FObjectFinder<UPCRSoundPrimaryDataAsset> DA_Sound(TEXT("/Script/ProjectCardReturn.PCRSoundPrimaryDataAsset'/Game/DataAssets/DA_Sound.DA_Sound'"));
	if (DA_Sound.Succeeded())
	{
		SoundDataAsset = DA_Sound.Object;
	}

	if (ParameterDataAsset)
	{
		GenerateInterval = ParameterDataAsset->SpawnerGenerateInterval;
	}

	if (StageDataAsset)
	{
		if (UClass* LiftBP = StageDataAsset->LiftBlueprint.LoadSynchronous())
		{
			LiftActorClass = LiftBP;
		}
	}
}

void APCRGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 블루프린트 에디터 실행 방지
#if WITH_EDITOR
	if (!GetWorld()->IsPlayInEditor())
	{
		return;
	}
#endif

	check(StageDataAsset && ParameterDataAsset && SoundDataAsset);

	CachedPCRGameInstance = Cast<UPCRGameInstance>(GetGameInstance());
	check(CachedPCRGameInstance);

	// 태그로 Lift를 찾는 코드입니다.
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Lift")), ActorsWithTag);
	if (!ActorsWithTag.IsEmpty())
	{
		APCRLiftActor* Lift = Cast<APCRLiftActor>(ActorsWithTag[0]);
		if (Lift)
		{
			LiftActor = Lift;
		}
	}

	check(LiftActor);
	LiftActor->OnLiftUpEnd.AddDynamic(this, &APCRGameModeBase::SpawnSerinDoll);

	OnStage1End.AddDynamic(this, &APCRGameModeBase::LiftFloor);
}

void APCRGameModeBase::StartPlay()
{
	Super::StartPlay();

	TArray<AActor*> EricaArray;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Erica")), EricaArray);
	if (!EricaArray.IsEmpty())
	{
		CachedEricaCharacter = Cast<APCREricaCharacter>(EricaArray[0]);
		check(CachedEricaCharacter);
	}
}

void APCRGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CachedPCRGameInstance->InitSoundSystem();
	CachedPCRGameInstance->InitInGameSoundSystem();

	SpawnMonsterGenerators();
	StartAllMonsterGenerators();

	CachedPCRGameInstance->PlayAmbientBGM();
	PlayStage1BGM();

	if (Stage1TargetKillCount == 0)
	{
		HandleKillCount();
	}
}

void APCRGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (CurrentStageState)
	{
		case EStageState::Stage:
		{
			break;
		}
		case EStageState::SerinStage:
		{
			break;
		}
	}
}

void APCRGameModeBase::SpawnSerinDoll()
{
	PlayBossStageBGM();
	FVector SpawnLocation = LiftActor->GetActorLocation();
	SpawnLocation.X += 1200.0;
	SpawnLocation.Z -= 250.0;
	SerinDollHead = GetWorld()->SpawnActor<APCRSerinDollHeadCharacter>(SpawnLocation, FRotator(0.0, 180.0, 0.0));

	CurrentStageState = EStageState::SerinStage;
}

void APCRGameModeBase::SpawnMonsterGenerators()
{
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(FVector(1500.0, -2000.0, 100.0), FRotator::ZeroRotator));
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(FVector(-1500.0, -2000.0, 100.0), FRotator::ZeroRotator));
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(FVector(1500.0, 2000.0, 100.0), FRotator::ZeroRotator));
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(FVector(-1500.0, 2000.0, 100.0), FRotator::ZeroRotator));

	for (const auto& MonsterGenerator : MonsterGenerators)
	{
		MonsterGenerator->OnSpawnedMonsterDead.BindUObject(this, &APCRGameModeBase::HandleKillCount);
	}
}

void APCRGameModeBase::StartAllMonsterGenerators()
{
	HandleSpawnMonster();

	SpawnMonsterDelegate.BindUObject(this, &APCRGameModeBase::HandleSpawnMonster);
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, SpawnMonsterDelegate, 7.0f, true);
}

void APCRGameModeBase::StopAllMonsterGeneratorsAndKillSpawnedMonsters()
{
	for (const auto& MonsterGenerator : MonsterGenerators)
	{
		MonsterGenerator->OnSpawnedMonsterDead.Unbind();

		MonsterGenerator->KillSpawnedMonsters();
	}
}

void APCRGameModeBase::HandleKillCount()
{
	++Stage1TotalMonsterKillCount;
	const int32 CurrentKillCount = Stage1TargetKillCount - Stage1TotalMonsterKillCount;
	OnChangeStage1MonsterCount.Execute(Stage1TargetKillCount, CurrentKillCount);

	// ToDo : 파라미터화 필요
	if (Stage1TotalMonsterKillCount >= 60 && Stage1TotalMonsterKillCount < 80)
	{
		Phase = 2;
	}
	else if (Stage1TotalMonsterKillCount >= 80 && Stage1TotalMonsterKillCount < Stage1TargetKillCount)
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, SpawnMonsterDelegate, 6.0f, true);
		Phase = 3;
	}
	else if (Stage1TotalMonsterKillCount >= Stage1TargetKillCount)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		StopAllMonsterGeneratorsAndKillSpawnedMonsters();
		OnStage1End.Broadcast();
	}
}

void APCRGameModeBase::HandleSpawnMonster()
{
	SpawnDefaultMonster();

	// ToDo : 파라미터화 필요
	switch (Phase)
	{
	case 1:
		if (Stage1DefaultMonsterSpawnCount % 15 == 0)
		{
			SpawnEliteMonster();
		}
		break;
	case 2:
		if (Stage1DefaultMonsterSpawnCount % 15 == 0)
		{
			SpawnEliteMonster();
		}
		break;
	case 3:
		if (Stage1DefaultMonsterSpawnCount % 10 == 0)
		{
			SpawnEliteMonster();
		}
		break;
	default:
		break;
	}
}

void APCRGameModeBase::SpawnDefaultMonster()
{
	// ToDo : 파라미터화 필요
	switch (Phase)
	{
	case 1:
		for (int i = 0; i < FMath::RandRange(2, 3); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRabbitCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;
		}
		for (int i = 0; i < FMath::RandRange(1, 3); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRMeleeSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;
		}
		for (int i = 0; i < FMath::RandRange(0, 1); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRangedSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;
		}
		break;
		
	case 2:
		for (int i = 0; i < FMath::RandRange(3, 5); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRabbitCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;
		}
		for (int i = 0; i < FMath::RandRange(2, 4); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRMeleeSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;
		}
		for (int i = 0; i < FMath::RandRange(0, 3); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRangedSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;
		}
		break;
		
	case 3:
		for (int i = 0; i < FMath::RandRange(3, 7); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRabbitCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;
		}
		for (int i = 0; i < FMath::RandRange(3, 7); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRMeleeSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;
		}
		for (int i = 0; i < FMath::RandRange(2, 5); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRangedSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;
		}
		break;
		
	default:
		break;
	}
}

void APCRGameModeBase::SpawnEliteMonster()
{
	UClass* EliteMonsterClass = GetEliteMonsterClass();

	if (Phase == 1)
	{
		if (Stage1EliteMonsterSpawnCount < 2)
		{
			EliteMonsterClass = APCREliteRabbitCharacter::StaticClass();
		}
		else
		{
			EliteMonsterClass = FMath::RandBool() ? APCREliteRabbitCharacter::StaticClass() : APCREliteMeleeSoldierCharacter::StaticClass();
		}
	}

	MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(EliteMonsterClass);
	++Stage1EliteMonsterSpawnCount;
}

UClass* APCRGameModeBase::GetEliteMonsterClass()
{
	// ToDo : 파라미터화 필요
	const float EliteRabbitRate = 40.0f;
	const float EliteMeleeSoldierRate = 40.0f;
	const float EliteRangedSoldierRate = 20.0f;

	const float RangeMax
		= EliteRabbitRate
		+ EliteMeleeSoldierRate
		+ EliteRangedSoldierRate;

	const int32 RandRange = FMath::RandRange(1, static_cast<int32>(RangeMax * 100));

	int32 Rate = EliteRabbitRate * 100;
	if (RandRange <= Rate)
	{
		return APCREliteRabbitCharacter::StaticClass();
	}
	Rate += EliteMeleeSoldierRate * 100;

	if (RandRange <= Rate)
	{
		return APCREliteMeleeSoldierCharacter::StaticClass();
	}
	
	return APCREliteRangedSoldierCharacter::StaticClass();
}

void APCRGameModeBase::PlayStage1BGM()
{
	CachedPCRGameInstance->PlayStage1BGM();
	CachedPCRGameInstance->StopBossStageBGM();
}

void APCRGameModeBase::PlayBossStageBGM()
{
	CachedPCRGameInstance->PlayBossStageBGM();
	CachedPCRGameInstance->StopStage1BGM();
}

void APCRGameModeBase::LiftFloor()
{
	CachedPCRGameInstance->StopStage1BGM();
	LiftActor->LiftUp();
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	check(PlayerCharacter);

	FVector NewLocation = LiftActor->GetActorLocation();
	NewLocation.Z = PlayerCharacter->GetActorLocation().Z;
	PlayerCharacter->SetActorLocation(NewLocation);
}
