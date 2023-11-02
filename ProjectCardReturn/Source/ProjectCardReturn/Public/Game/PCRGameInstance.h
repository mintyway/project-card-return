// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/GameInstance.h"
#include "PCRGameInstance.generated.h"

namespace FMOD::Studio
{
	class Bus;
	class System;
}

class APCREricaCardProjectilePool;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

public:
	void InitSoundSystem();
	
private:
	bool SoundUpdate(float DeltaTime);
	
private:
	FMOD::Studio::System* FMODStudioSystem;
	FMOD::Studio::Bus* MasterBus;

private:
	float MasterVolume;
};
