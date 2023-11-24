// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRGameInstance.h"

#include "Game/PCRSoundPrimaryDataAsset.h"

#include "FMODBlueprintStatics.h"
#include "FMODStudioModule.h"
#include "fmod_studio.hpp"
#include "Components/Widget.h"
#include "Entities/Boss/SerinDoll/Head/PCRSerinDollHeadCharacter.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Entities/Stage/Lift/PCRLiftActor.h"
#include "Game/PCRGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(PCRLogGameInstance);

UPCRGameInstance::UPCRGameInstance()
	: FMODStudioSystem(nullptr), MasterBus(nullptr), MainAudioInst(nullptr),
	  AmbientAudioInst(nullptr), Stage1AudioInst(nullptr), BossStageAudioInst(nullptr), EndingAudioInst(nullptr),
	  bIsInit(false), LastMasterVolume(100.f)
{
	static ConstructorHelpers::FObjectFinder<UPCRSoundPrimaryDataAsset> DA_Sound(TEXT("/Script/ProjectCardReturn.PCRSoundPrimaryDataAsset'/Game/DataAssets/DA_Sound.DA_Sound'"));
	if (DA_Sound.Succeeded())
	{
		SoundDataAsset = DA_Sound.Object;
	}
}

void UPCRGameInstance::Init()
{
	Super::Init();
	
	check(SoundDataAsset);

	FMODStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
	check(FMODStudioSystem);

	FMOD::Studio::Bus* MasterBusRawPtr;
	FMODStudioSystem->getBus("bus:/", &MasterBusRawPtr);
	MasterBus = MasterBusRawPtr;
	check(MasterBus);
}

void UPCRGameInstance::OnStart()
{
	Super::OnStart();

	InitInGameSoundSystem();
}

void UPCRGameInstance::InitInGameSoundSystem()
{
	ReleaseInGameSoundSystem();
	const FFMODEventInstance MainAudio = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->MainBGM, false);
	MainAudioInst = MainAudio.Instance;
	check(MainAudioInst);
	MainAudioInst->setVolume(0.25f);

	const FFMODEventInstance AmbientAudio = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->AmbientBGM, false);
	AmbientAudioInst = AmbientAudio.Instance;
	check(AmbientAudioInst);
	AmbientAudioInst->setVolume(0.75f);

	const FFMODEventInstance Stage1Audio = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->Stage1BGM, false);
	Stage1AudioInst = Stage1Audio.Instance;
	check(Stage1AudioInst);
	Stage1AudioInst->setVolume(0.25f);

	const FFMODEventInstance BossStageAudio = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->BossStageBGM, false);
	BossStageAudioInst = BossStageAudio.Instance;
	check(BossStageAudioInst);
	BossStageAudioInst->setVolume(0.25f);

	const FFMODEventInstance EndingAudio = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->EndingBGM, false);
	EndingAudioInst = EndingAudio.Instance;
	check(EndingAudioInst);
	EndingAudioInst->setVolume(0.25f);
}

void UPCRGameInstance::ReleaseInGameSoundSystem()
{
	if (MainAudioInst)
	{
		MainAudioInst->release();
	}
	
	if (AmbientAudioInst)
	{
		AmbientAudioInst->release();
	}

	if (Stage1AudioInst)
	{
		Stage1AudioInst->release();
	}

	if (BossStageAudioInst)
	{
		BossStageAudioInst->release();
	}

	if (EndingAudioInst)
	{
		EndingAudioInst->release();
	}
}

float UPCRGameInstance::GetMasterVolume() const
{
	float CurrentVolume;
	MasterBus->getVolume(&CurrentVolume);

	return CurrentVolume;
}

void UPCRGameInstance::SetMasterVolume(float InVolume)
{
	const float NewVolume = FMath::Clamp(InVolume, 0.f, 1.f);
	MasterBus->setVolume(NewVolume);
	LastMasterVolume = GetMasterVolume();
}

void UPCRGameInstance::MasterVolumeOn()
{
	SetMasterVolume(LastMasterVolume);
}

void UPCRGameInstance::MasterVolumeOff()
{
	LastMasterVolume = GetMasterVolume();
	MasterBus->setVolume(0.f);
}

void UPCRGameInstance::PlayMainBGM()
{
	MainAudioInst->start();
}

void UPCRGameInstance::StopMainBGM()
{
	MainAudioInst->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void UPCRGameInstance::PlayStage1BGM()
{
	Stage1AudioInst->start();
}

void UPCRGameInstance::StopStage1BGM()
{
	Stage1AudioInst->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void UPCRGameInstance::PlayAmbientBGM()
{
	AmbientAudioInst->start();
}

void UPCRGameInstance::StopAmbientBGM()
{
	AmbientAudioInst->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void UPCRGameInstance::PlayBossStageBGM()
{
	BossStageAudioInst->start();
}

void UPCRGameInstance::StopBossStageBGM()
{
	BossStageAudioInst->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void UPCRGameInstance::PlayEndingBGM()
{
	EndingAudioInst->start();;
}

void UPCRGameInstance::StopEndingBGM()
{
	EndingAudioInst->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

void UPCRGameInstance::RestartGame(UWidget* Widget)
{
	const UWorld* CurrentWorld = GetWorld();
	APCRGameModeBase* PCRGameMode = Cast<APCRGameModeBase>(CurrentWorld->GetAuthGameMode());
	if (PCRGameMode)
	{
		if (PCRGameMode->GetCurrentStageState() == EStageState::SerinStage)
		{
			Widget->RemoveFromParent();
			PCRGameMode->GetSerinDollHead()->DestroySerinDoll();
			PCRGameMode->SpawnSerinDoll();

			APCREricaCharacter* Erica = PCRGameMode->GetCachedEricaCharacter();
			check(Erica);
			Erica->Revival();
			const APCRLiftActor* LiftActor = PCRGameMode->GetLiftActor();
			const FVector LiftLocation = LiftActor->GetActorLocation();
			FVector NewLocation = PCRGameMode->GetCachedEricaCharacter()->GetActorLocation();
			NewLocation.X = LiftLocation.X;
			NewLocation.Y = LiftLocation.Y;
			PCRGameMode->GetCachedEricaCharacter()->SetActorLocation(NewLocation);

			OnRestart.Broadcast();
			
			UE_LOG(PCRLogGameInstance, Log, TEXT("보스전투가 재시작되었습니다."));

			return;
		}
	}

	const FString CurrentLevelName = CurrentWorld->GetName();
	UGameplayStatics::OpenLevel(CurrentWorld, *CurrentLevelName);

	UE_LOG(PCRLogGameInstance, Log, TEXT("게임이 재시작되었습니다."));
}

void UPCRGameInstance::ToMain()
{
	StopAmbientBGM();
	StopStage1BGM();
	StopBossStageBGM();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Main"));

	UE_LOG(PCRLogGameInstance, Log, TEXT("메인 화면으로 이동합니다."));
}

void UPCRGameInstance::QuitGame()
{
	ReleaseInGameSoundSystem();
	
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
