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
	virtual void Shutdown() override;

public: // 동작
	void InitSoundSystem();
	void InitInGameSoundSystem();
	void ReleaseInGameSoundSystem();

	float GetMasterVolume() const;
	void SetMasterVolume(float InVolume);
	void MasterVolumeOn();
	void MasterVolumeOff();

	UFUNCTION(BlueprintCallable)
	void PlayStage1BGM();
	UFUNCTION(BlueprintCallable)
	void StopStage1BGM();

	UFUNCTION(BlueprintCallable)
	void PlayAmbientBGM();
	UFUNCTION(BlueprintCallable)
	void StopAmbientBGM();

	UFUNCTION(BlueprintCallable)
	void PlayBossStageBGM();
	UFUNCTION(BlueprintCallable)
	void StopBossStageBGM();

private:
	bool SoundUpdate(float DeltaTime);
	FORCEINLINE float GetLastMasterVolume() const { return LastMasterVolume; }

	// 데이터 에셋
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
	float LastMasterVolume;
};
