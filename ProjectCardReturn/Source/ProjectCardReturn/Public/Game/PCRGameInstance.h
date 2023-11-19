// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/GameInstance.h"
#include "PCRGameInstance.generated.h"

class UPCRSoundPrimaryDataAsset;
class APCREricaCardProjectilePool;
namespace FMOD::Studio
{
	class Bus;
	class System;
	class EventInstance;
}

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPCRGameInstance();

protected:
	virtual void Init() override;

public: // 동작
	void InitSoundSystem();
	void InitInGameSoundSystem();
	
	void PlayStage1BGM();
	void StopStage1BGM();
	
	void PlayAmbientBGM();
	void StopAmbientBGM();
	
	void PlayBossStageBGM();
	void StopBossStageBGM();
	
private:
	bool SoundUpdate(float DeltaTime);

private: // 데이터 에셋
	UPROPERTY()
	TObjectPtr<const UPCRSoundPrimaryDataAsset> SoundDataAsset;
	
private: // 오디오
	FMOD::Studio::System* FMODStudioSystem;
	FMOD::Studio::Bus* MasterBus;
	
	FMOD::Studio::EventInstance* AmbientAudioInst;
	FMOD::Studio::EventInstance* Stage1AudioInst;
	FMOD::Studio::EventInstance* BossStageAudioInst;

private: // 데이터
	float MasterVolume;
};
