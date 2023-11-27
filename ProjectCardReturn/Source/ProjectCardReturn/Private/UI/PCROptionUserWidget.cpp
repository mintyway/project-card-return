// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCROptionUserWidget.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Game/PCRGameInstance.h"

void UPCROptionUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CachedPCRGameInstance = Cast<UPCRGameInstance>(GetGameInstance());
	check(CachedPCRGameInstance);
	
	SLD_SoundBar = Cast<USlider>(GetWidgetFromName(TEXT("SLD_SoundBar")));
	check(SLD_SoundBar);

	PB_SoundBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_SoundBar")));
	check(PB_SoundBar);

	BT_SoundOn = Cast<UButton>(GetWidgetFromName(TEXT("BT_SoundOn")));
	check(BT_SoundOn);

	BT_SoundOff = Cast<UButton>(GetWidgetFromName(TEXT("BT_SoundOff")));
	check(BT_SoundOff);

	const float VolumeRatio = CachedPCRGameInstance->GetMasterVolume();
	SLD_SoundBar->SetValue(VolumeRatio);
	PB_SoundBar->SetPercent(VolumeRatio);
	HandleSoundImageOnOff();
	
	SLD_SoundBar->OnValueChanged.AddDynamic(this, &UPCROptionUserWidget::HandleChangeSoundBar);
	BT_SoundOn->OnClicked.AddDynamic(this, &UPCROptionUserWidget::HandleSoundOff);
	BT_SoundOff->OnClicked.AddDynamic(this, &UPCROptionUserWidget::HandleSoundOn);
}

void UPCROptionUserWidget::HandleChangeSoundBar(float Value)
{
	PB_SoundBar->SetPercent(Value);
	CachedPCRGameInstance->SetMasterVolume(Value);
	HandleSoundImageOnOff();
}

void UPCROptionUserWidget::HandleSoundOn()
{
	CachedPCRGameInstance->MasterVolumeOn();
	HandleSoundImageOnOff();
}

void UPCROptionUserWidget::HandleSoundOff()
{
	CachedPCRGameInstance->MasterVolumeOff();
	HandleSoundImageOnOff();
}

void UPCROptionUserWidget::HandleSoundImageOnOff()
{
	const float MasterVolume = CachedPCRGameInstance->GetMasterVolume();

	if (MasterVolume > 0.f)
	{
		BT_SoundOn->SetVisibility(ESlateVisibility::Visible);
		BT_SoundOff->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		BT_SoundOn->SetVisibility(ESlateVisibility::Hidden);
		BT_SoundOff->SetVisibility(ESlateVisibility::Visible);
	}
}
