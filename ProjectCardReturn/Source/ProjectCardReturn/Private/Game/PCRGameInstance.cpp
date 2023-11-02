// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PCRGameInstance.h"

#include "FMODStudioModule.h"
#include "fmod_studio.hpp"

void UPCRGameInstance::Init()
{
	Super::Init();

	MasterVolume = 1.f;
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

bool UPCRGameInstance::SoundUpdate(float DeltaTime)
{
	SIMPLE_LOG;
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
