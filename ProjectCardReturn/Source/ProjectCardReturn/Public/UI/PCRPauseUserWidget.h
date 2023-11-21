// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Blueprint/UserWidget.h"
#include "PCRPauseUserWidget.generated.h"

class UPCRGameInstance;
class UProgressBar;
class USlider;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRPauseUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void HandleResumeClicked();

	UFUNCTION()
	void HandleRestartClicked();

	UFUNCTION()
	void HandleQuitClicked();

	UFUNCTION()
	void HandleChangeSoundBar(float Value);

	UFUNCTION()
	void HandleSoundOn();

	UFUNCTION()
	void HandleSoundOff();

	void HandleSoundImageOnOff();

private: // 데이터
	UPROPERTY()
	TObjectPtr<UPCRGameInstance> CachedPCRGameInstance;

	UPROPERTY()
	TObjectPtr<UButton> BT_Resume;

	UPROPERTY()
	TObjectPtr<UButton> BT_Restart;

	UPROPERTY()
	TObjectPtr<UButton> BT_Quit;

	UPROPERTY()
	TObjectPtr<USlider> SLD_SoundBar;

	UPROPERTY()
	TObjectPtr<UProgressBar> PB_SoundBar;

	UPROPERTY()
	TObjectPtr<UButton> BT_SoundOn;

	UPROPERTY()
	TObjectPtr<UButton> BT_SoundOff;
};
