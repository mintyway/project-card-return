// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/GameInstance.h"
#include "PCRGameInstance.generated.h"

namespace FMOD::Studio
{
	class Bus;
	class System;
	class EventInstance;
}

class UPCRSoundPrimaryDataAsset;
class APCREricaCardProjectilePool;

DECLARE_LOG_CATEGORY_EXTERN(PCRLogGameInstance, Log, All);

DECLARE_MULTICAST_DELEGATE(FRestartSignature);

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
	virtual void OnStart() override;

public: // 동작
	void InitInGameSoundSystem();
	void ReleaseInGameSoundSystem();

	float GetMasterVolume() const;
	void SetMasterVolume(float InVolume);
	void MasterVolumeOn();
	void MasterVolumeOff();

	UFUNCTION(BlueprintCallable)
	void PlayMainBGM();

	UFUNCTION(BlueprintCallable)
	void StopMainBGM();

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

	UFUNCTION(BlueprintCallable)
	void PlayEndingBGM();

	UFUNCTION(BlueprintCallable)
	void StopEndingBGM();

	UFUNCTION(BlueprintCallable)
	void RestartGame(UWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void ToMain();

	UFUNCTION(BlueprintCallable)
	void QuitGame();

public: // 델리게이트
	FRestartSignature OnRestart;

private:
	FORCEINLINE float GetLastMasterVolume() const { return LastMasterVolume; }

	// 데이터 에셋
	UPROPERTY()
	TObjectPtr<const UPCRSoundPrimaryDataAsset> SoundDataAsset;

private: // 오디오
	FMOD::Studio::System* FMODStudioSystem;
	FMOD::Studio::Bus* MasterBus;
	
	FMOD::Studio::EventInstance* MainAudioInst;
	FMOD::Studio::EventInstance* AmbientAudioInst;
	FMOD::Studio::EventInstance* Stage1AudioInst;
	FMOD::Studio::EventInstance* BossStageAudioInst;
	FMOD::Studio::EventInstance* EndingAudioInst;

private: // 데이터
	uint32 bIsInit:1;
	float LastMasterVolume;
};
