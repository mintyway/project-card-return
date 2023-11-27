// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Blueprint/UserWidget.h"
#include "PCROptionUserWidget.generated.h"

class UPCRGameInstance;
class USlider;
class UProgressBar;
class UButton;

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCROptionUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
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
	TObjectPtr<USlider> SLD_SoundBar;

	UPROPERTY()
	TObjectPtr<UProgressBar> PB_SoundBar;

	UPROPERTY()
	TObjectPtr<UButton> BT_SoundOn;

	UPROPERTY()
	TObjectPtr<UButton> BT_SoundOff;
};
