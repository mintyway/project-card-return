// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRGameInstance.h"

#include "Game/PCRSoundPrimaryDataAsset.h"

#include "FMODBlueprintStatics.h"
#include "FMODStudioModule.h"
#include "fmod_studio.hpp"

UPCRGameInstance::UPCRGameInstance()
	: FMODStudioSystem(nullptr), MasterBus(nullptr),
	  AmbientAudioInst(nullptr), Stage1AudioInst(nullptr), BossStageAudioInst(nullptr),
	  MasterVolume(0.f)
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

	MasterVolume = 0.5f;
}

void UPCRGameInstance::InitSoundSystem()
{
	FMODStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
	check(FMODStudioSystem);

	FMOD::Studio::Bus* MasterBusRawPtr;
	FMODStudioSystem->getBus("bus:/", &MasterBusRawPtr);
	MasterBus = MasterBusRawPtr;
	check(MasterBus);

	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UPCRGameInstance::SoundUpdate));
}

void UPCRGameInstance::InitInGameSoundSystem()
{
	if (AmbientAudioInst)
	{
		AmbientAudioInst->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	}

	if (Stage1AudioInst)
	{
		Stage1AudioInst->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	}

	if (BossStageAudioInst)
	{
		BossStageAudioInst->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	}
	
	const FFMODEventInstance AmbientAudio = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->AmbientBGM, false);
	AmbientAudioInst = AmbientAudio.Instance;
	if (AmbientAudioInst)
	{
		AmbientAudioInst->setVolume(0.75f);
	}

	const FFMODEventInstance Stage1Audio = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->Stage1BGM, false);
	Stage1AudioInst = Stage1Audio.Instance;
	if (Stage1AudioInst)
	{
		Stage1AudioInst->setVolume(0.2f);
	}

	const FFMODEventInstance BossStageAudio = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), SoundDataAsset->BossStageBGM, false);
	BossStageAudioInst = BossStageAudio.Instance;
	if (BossStageAudioInst)
	{
		BossStageAudioInst->setVolume(0.2f);
	}
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

bool UPCRGameInstance::SoundUpdate(float DeltaTime)
{
	float CurrentVolume;
	MasterBus->getVolume(&CurrentVolume);
	if (CurrentVolume != MasterVolume)
	{
		if (MasterBus)
		{
			MasterBus->setVolume(MasterVolume);
		}
	}

	return true;
}
