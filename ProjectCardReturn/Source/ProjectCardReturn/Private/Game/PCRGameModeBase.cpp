// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRGameModeBase.h"

#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
#include "Entities/Item/PCRHealItem.h"
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
	: Phase(1), Stage1DefaultMonsterSpawnCount(0), Stage1EliteMonsterSpawnCount(0), Stage1TotalMonsterKillCount(0), CurrentStageState(EStageState::Stage)
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
		Stage1TargetKillCount =
			ParameterDataAsset->MonsterTargetKillCountInPhase1 +
			ParameterDataAsset->MonsterTargetKillCountInPhase2 +
			ParameterDataAsset->MonsterTargetKillCountInPhase3;
	}

	if (StageDataAsset)
	{
		if (UClass* LiftBP = StageDataAsset->LiftBlueprint.LoadSynchronous())
		{
			LiftActorClass = LiftBP;
		}
	}

	LastPhase = Phase;
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

	// TODO: 시퀀스 재생 이벤트에 맞게 리프트 상승 필요함.
	// LiftActor->OnLiftUpEnd.AddDynamic(this, &APCRGameModeBase::SpawnSerinDoll);

	// TODO: 시퀀스 재생 이벤트에 맞게 리프트 상승 필요함.
	// OnStage1End.AddDynamic(this, &APCRGameModeBase::LiftFloor);
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

	SpawnMonsterGenerators();
	StartAllMonsterGenerators();

	CachedPCRGameInstance->StopMainBGM();
	CachedPCRGameInstance->PlayAmbientBGM();
	PlayStage1BGM();

	if (Stage1TargetKillCount == 0)
	{
		HandleKillCount();
	}

	for (int i = 0; i < 3; i++)
	{
		HealItems.Emplace(nullptr);
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
	SpawnLocation.X += ParameterDataAsset->SerinSpawnLocationOffset.X;
	SpawnLocation.Y += ParameterDataAsset->SerinSpawnLocationOffset.Y;
	SpawnLocation.Z += ParameterDataAsset->SerinSpawnLocationOffset.Z;
	SerinDollHead = GetWorld()->SpawnActor<APCRSerinDollHeadCharacter>(SpawnLocation, FRotator(0.0, 180.0, 0.0));

	CurrentStageState = EStageState::SerinStage;

	OnSpawnSerin.Broadcast();
}

void APCRGameModeBase::SpawnMonsterGenerators()
{
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(ParameterDataAsset->MonsterGeneratorLocation1, FRotator::ZeroRotator));
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(ParameterDataAsset->MonsterGeneratorLocation2, FRotator::ZeroRotator));
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(ParameterDataAsset->MonsterGeneratorLocation3, FRotator::ZeroRotator));
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(ParameterDataAsset->MonsterGeneratorLocation4, FRotator::ZeroRotator));

	for (const auto& MonsterGenerator : MonsterGenerators)
	{
		MonsterGenerator->OnSpawnedMonsterDead.BindUObject(this, &APCRGameModeBase::HandleKillCount);
	}
}

void APCRGameModeBase::StartAllMonsterGenerators()
{
	HandleSpawnMonster();

	SpawnMonsterDelegate.BindUObject(this, &APCRGameModeBase::HandleSpawnMonster);
	GetWorldTimerManager().SetTimer(SpawnMonsterTimerHandle, SpawnMonsterDelegate, ParameterDataAsset->MonsterSpawnTimeInPhase1, true);
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

	const int32 TargetKillCountInPhase1 = ParameterDataAsset->MonsterTargetKillCountInPhase1;
	const int32 TargetKillCountInPhase2 = ParameterDataAsset->MonsterTargetKillCountInPhase2;
	
	if (Stage1TotalMonsterKillCount >= TargetKillCountInPhase1 && Stage1TotalMonsterKillCount < TargetKillCountInPhase1 + TargetKillCountInPhase2)
	{
		Phase = 2;
		if (LastPhase != Phase)
		{
			GetWorldTimerManager().ClearTimer(SpawnMonsterTimerHandle);
			GetWorldTimerManager().SetTimer(SpawnMonsterTimerHandle, SpawnMonsterDelegate, ParameterDataAsset->MonsterSpawnTimeInPhase2, true);
			LastPhase = Phase;
		}
	}
	else if (Stage1TotalMonsterKillCount >= TargetKillCountInPhase1 + TargetKillCountInPhase2 && Stage1TotalMonsterKillCount < Stage1TargetKillCount)
	{
		Phase = 3;
		if (LastPhase != Phase)
		{
			GetWorldTimerManager().ClearTimer(SpawnMonsterTimerHandle);
			GetWorldTimerManager().SetTimer(SpawnMonsterTimerHandle, SpawnMonsterDelegate, ParameterDataAsset->MonsterSpawnTimeInPhase3, true);
			LastPhase = Phase;
		}
	}
	else if (Stage1TotalMonsterKillCount >= Stage1TargetKillCount)
	{
		Phase = 4;
		if (LastPhase != Phase)
		{
			GetWorldTimerManager().ClearTimer(SpawnMonsterTimerHandle);
			StopAllMonsterGeneratorsAndKillSpawnedMonsters();
			FVector NewLocation = FVector(-1105.0, 0.0, 0.0);
			NewLocation.Z = CachedEricaCharacter->GetActorLocation().Z;
			CachedEricaCharacter->SetActorLocation(NewLocation);
			OnStage1End.Broadcast();
			LastPhase = Phase;
		}
	}
}

void APCRGameModeBase::HandleSpawnMonster()
{
	switch (Phase)
	{
	case 1:
		for (int i = 0; i < FMath::RandRange(ParameterDataAsset->RabbitMinSpawnCountByOnceInPhase1,
			ParameterDataAsset->RabbitMaxSpawnCountByOnceInPhase1); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRabbitCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->EliteMonsterSpawnPerCountInPhase1 == 0)
			{
				SpawnEliteMonster();
			}

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->HealItemSpawnPerCountByGame == 0)
			{
				SpawnHealItem();
			}
		}
		for (int i = 0; i < FMath::RandRange(ParameterDataAsset->MeleeSoldierMinSpawnCountByOnceInPhase1,
			ParameterDataAsset->MeleeSoldierMaxSpawnCountByOnceInPhase1); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRMeleeSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->EliteMonsterSpawnPerCountInPhase1 == 0)
			{
				SpawnEliteMonster();
			}

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->HealItemSpawnPerCountByGame == 0)
			{
				SpawnHealItem();
			}
		}
		for (int i = 0; i < FMath::RandRange(ParameterDataAsset->RangedSoldierMinSpawnCountByOnceInPhase1,
			ParameterDataAsset->RangedSoldierMaxSpawnCountByOnceInPhase1); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRangedSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->EliteMonsterSpawnPerCountInPhase1 == 0)
			{
				SpawnEliteMonster();
			}

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->HealItemSpawnPerCountByGame == 0)
			{
				SpawnHealItem();
			}
		}
		break;
		
	case 2:
		for (int i = 0; i < FMath::RandRange(ParameterDataAsset->RabbitMinSpawnCountByOnceInPhase2,
			ParameterDataAsset->RabbitMaxSpawnCountByOnceInPhase2); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRabbitCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->EliteMonsterSpawnPerCountInPhase2 == 0)
			{
				SpawnEliteMonster();
			}

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->HealItemSpawnPerCountByGame == 0)
			{
				SpawnHealItem();
			}
		}
		for (int i = 0; i < FMath::RandRange(ParameterDataAsset->MeleeSoldierMinSpawnCountByOnceInPhase2,
			ParameterDataAsset->MeleeSoldierMaxSpawnCountByOnceInPhase2); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRMeleeSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->EliteMonsterSpawnPerCountInPhase2 == 0)
			{
				SpawnEliteMonster();
			}

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->HealItemSpawnPerCountByGame == 0)
			{
				SpawnHealItem();
			}
		}
		for (int i = 0; i < FMath::RandRange(ParameterDataAsset->RangedSoldierMinSpawnCountByOnceInPhase2,
			ParameterDataAsset->RangedSoldierMaxSpawnCountByOnceInPhase2); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRangedSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->EliteMonsterSpawnPerCountInPhase2 == 0)
			{
				SpawnEliteMonster();
			}

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->HealItemSpawnPerCountByGame == 0)
			{
				SpawnHealItem();
			}
		}
		break;
		
	case 3:
		for (int i = 0; i < FMath::RandRange(ParameterDataAsset->RabbitMinSpawnCountByOnceInPhase3,
			ParameterDataAsset->RabbitMaxSpawnCountByOnceInPhase3); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRabbitCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->EliteMonsterSpawnPerCountInPhase3 == 0)
			{
				SpawnEliteMonster();
			}

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->HealItemSpawnPerCountByGame == 0)
			{
				SpawnHealItem();
			}
		}
		for (int i = 0; i < FMath::RandRange(ParameterDataAsset->MeleeSoldierMinSpawnCountByOnceInPhase3,
			ParameterDataAsset->MeleeSoldierMaxSpawnCountByOnceInPhase3); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRMeleeSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->EliteMonsterSpawnPerCountInPhase3 == 0)
			{
				SpawnEliteMonster();
			}

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->HealItemSpawnPerCountByGame == 0)
			{
				SpawnHealItem();
			}
		}
		for (int i = 0; i < FMath::RandRange(ParameterDataAsset->RangedSoldierMinSpawnCountByOnceInPhase3,
			ParameterDataAsset->RangedSoldierMaxSpawnCountByOnceInPhase3); i++)
		{
			MonsterGenerators[FMath::RandRange(0, 3)]->SpawnMonster(APCRRangedSoldierCharacter::StaticClass());
			++Stage1DefaultMonsterSpawnCount;

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->EliteMonsterSpawnPerCountInPhase3 == 0)
			{
				SpawnEliteMonster();
			}

			if (Stage1DefaultMonsterSpawnCount % ParameterDataAsset->HealItemSpawnPerCountByGame == 0)
			{
				SpawnHealItem();
			}
		}
		break;
		
	default:
		break;
	}
}

void APCRGameModeBase::HandleHealItemOverlap(APCRHealItem* HealItem)
{
	HealItems[HealItems.Find(HealItem)] = nullptr;
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

void APCRGameModeBase::SpawnHealItem()
{
	if (!HealItems[0])
	{
		HealItems[0] = GetWorld()->SpawnActor<APCRHealItem>(ParameterDataAsset->HealItemSpawnLocationByGame1, FRotator(0.0, 90.0, 0.0));
		HealItems[0]->OnOverlapDelegate.AddUObject(this, &APCRGameModeBase::HandleHealItemOverlap);
	}

	if (!HealItems[1])
	{
		HealItems[1] = GetWorld()->SpawnActor<APCRHealItem>(ParameterDataAsset->HealItemSpawnLocationByGame2, FRotator(0.0, 90.0, 0.0));
		HealItems[1]->OnOverlapDelegate.AddUObject(this, &APCRGameModeBase::HandleHealItemOverlap);
	}

	if (!HealItems[2])
	{
		HealItems[2] = GetWorld()->SpawnActor<APCRHealItem>(ParameterDataAsset->HealItemSpawnLocationByGame3, FRotator(0.0, 90.0, 0.0));
		HealItems[2]->OnOverlapDelegate.AddUObject(this, &APCRGameModeBase::HandleHealItemOverlap);
	}
}

UClass* APCRGameModeBase::GetEliteMonsterClass()
{
	const float EliteRabbitRate = ParameterDataAsset->EliteRabbitSpawnRate;
	const float EliteMeleeSoldierRate = ParameterDataAsset->EliteMeleeSoldierSpawnRate;
	const float EliteRangedSoldierRate = ParameterDataAsset->EliteRangedSoldierSpawnRate;

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
