// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRGameModeBase.h"

#include "FMODAudioComponent.h"
#include "FMODStudioModule.h"
#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
#include "Entities/Stage/Base/PCRStagePrimaryDataAsset.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Players/Erica/PCREricaPlayerController.h"
#include "Entities/Monsters/Rabbit/PCRRabbitCharacter.h"
#include "Entities/MonsterGenerator/PCRMonsterGenerator.h"
#include "Entities/Monsters/Soldier/MeleeSoldier/PCRMeleeSoldierCharacter.h"
#include "Entities/Monsters/Soldier/RangedSoldier/PCRRangedSoldierCharacter.h"
#include "Game/PCRGameInstance.h"
#include "Game/PCRParameterDataAsset.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(PCRLogGameModeBase);

APCRGameModeBase::APCRGameModeBase(): TotalMonsterKillCount(0), Stage1TargetKillCount(1), CurrentStageNumber(EStageNumber::Stage1)
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
		UClass* LiftBP = StageDataAsset->LiftBlueprint.LoadSynchronous();
		if (LiftBP)
		{
			LiftActorClass = LiftBP;
		}
	}

	AmbientAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("AmbientAudioComponent"));
	if (AmbientAudioComponent)
	{
		AmbientAudioComponent->SetEvent(SoundDataAsset->AmbientBGM);
	}

	Stage1AudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Stage1AudioComponent"));
	if (Stage1AudioComponent)
	{
		Stage1AudioComponent->SetEvent(SoundDataAsset->Stage1BGM);
	}

	BossStageAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("BossStageAudioComponent"));
	if (BossStageAudioComponent)
	{
		BossStageAudioComponent->SetEvent(SoundDataAsset->BossStageBGM);
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

	AmbientAudioComponent->SetVolume(0.75f);
	Stage1AudioComponent->SetVolume(0.2f);
	BossStageAudioComponent->SetVolume(0.2f);
	Stage1AudioComponent->Deactivate();
	BossStageAudioComponent->Deactivate();

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

void APCRGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UPCRGameInstance* PCRGameInstance = Cast<UPCRGameInstance>(GetGameInstance());
	check(PCRGameInstance);
	PCRGameInstance->InitSoundSystem();

	SpawnMonsterGenerators();
	StartAllMonsterGenerators();

	PlayStage1BGM();
}

void APCRGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (CurrentStageNumber)
	{
		case EStageNumber::Stage1:
		{
			break;
		}
		case EStageNumber::Stage2:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void APCRGameModeBase::SpawnMonsterGenerators()
{
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(FVector(1500.0, 0.0, 100.0), FRotator::ZeroRotator));
	MonsterGenerators.Emplace(GetWorld()->SpawnActor<APCRMonsterGenerator>(FVector(-1500.0, 0.0, 100.0), FRotator::ZeroRotator));
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
	MonsterGenerators[0]->Start(APCRMeleeSoldierCharacter::StaticClass(), GenerateInterval);
	MonsterGenerators[1]->Start(APCRRangedSoldierCharacter::StaticClass(), GenerateInterval);
	MonsterGenerators[2]->Start(APCRMeleeSoldierCharacter::StaticClass(), GenerateInterval);
	MonsterGenerators[3]->Start(APCRMeleeSoldierCharacter::StaticClass(), GenerateInterval);
	MonsterGenerators[4]->Start(APCRRabbitCharacter::StaticClass(), GenerateInterval);
	MonsterGenerators[5]->Start(APCRRabbitCharacter::StaticClass(), GenerateInterval);
}

void APCRGameModeBase::StopAllMonsterGeneratorsAndKillSpawnedMonsters()
{
	for (const auto& MonsterGenerator : MonsterGenerators)
	{
		MonsterGenerator->Stop();
		MonsterGenerator->OnSpawnedMonsterDead.Unbind();

		MonsterGenerator->KillSpawnedMonsters();
	}
}

void APCRGameModeBase::DisplayLogMonsterKillCount()
{
	UE_LOG(PCRLogGameModeBase, Warning, TEXT("처치한 몬스터 수: %d / %d"), TotalMonsterKillCount, Stage1TargetKillCount);
}

void APCRGameModeBase::HandleKillCount()
{
	++TotalMonsterKillCount;
	DisplayLogMonsterKillCount();

	if (TotalMonsterKillCount >= Stage1TargetKillCount)
	{
		StopAllMonsterGeneratorsAndKillSpawnedMonsters();
		OnStage1End.Broadcast();
	}
}

void APCRGameModeBase::PlayStage1BGM()
{
	Stage1AudioComponent->Activate(true);
	BossStageAudioComponent->Deactivate();
}

void APCRGameModeBase::PlayBossStageBGM()
{
	Stage1AudioComponent->Deactivate();
	BossStageAudioComponent->Activate(true);
}

void APCRGameModeBase::LiftFloor()
{
	Stage1AudioComponent->Deactivate();
	LiftActor->LiftUp();
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	check(PlayerCharacter);

	const FVector ResetLocation = FVector(-1400.0, 0.0, PlayerCharacter->GetActorLocation().Z);
	PlayerCharacter->SetActorLocation(ResetLocation);

	CurrentStageNumber = EStageNumber::Stage2;
}

void APCRGameModeBase::SpawnSerinDoll()
{
	PlayBossStageBGM();
	FVector SpawnLocation = LiftActor->GetActorLocation();
	SpawnLocation.X += 1200.0;
	SpawnLocation.Z -= 250.0;
	GetWorld()->SpawnActor<APCRSerinDollHeadCharacter>(SpawnLocation, FRotator(0.0, 180.0, 0.0));
}
