// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRPauseUserWidget.h"

#include "Entities/Players/Erica/PCREricaPlayerController.h"

#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Game/PCRGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPCRPauseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Resume = Cast<UButton>(GetWidgetFromName(TEXT("BT_Resume")));
	check(BT_Resume);

	BT_Restart = Cast<UButton>(GetWidgetFromName(TEXT("BT_Restart")));
	check(BT_Restart);

	BT_Quit = Cast<UButton>(GetWidgetFromName(TEXT("BT_Quit")));
	check(BT_Quit);

	SLD_SoundBar = Cast<USlider>(GetWidgetFromName(TEXT("SLD_SoundBar")));
	check(SLD_SoundBar);

	PB_SoundBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_SoundBar")));
	check(PB_SoundBar);

	BT_SoundOn = Cast<UButton>(GetWidgetFromName(TEXT("BT_SoundOn")));
	check(BT_SoundOn);

	BT_SoundOff = Cast<UButton>(GetWidgetFromName(TEXT("BT_SoundOff")));
	check(BT_SoundOff);

	CachedPCRGameInstance = Cast<UPCRGameInstance>(GetGameInstance());
	check(CachedPCRGameInstance);

	const float VolumeRatio = CachedPCRGameInstance->GetMasterVolume();
	SLD_SoundBar->SetValue(VolumeRatio);
	PB_SoundBar->SetPercent(VolumeRatio);
	HandleSoundImageOnOff();

	BT_Resume->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleResumeClicked);
	BT_Restart->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleRestartClicked);
	BT_Quit->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleQuitClicked);
	SLD_SoundBar->OnValueChanged.AddDynamic(this, &UPCRPauseUserWidget::HandleChangeSoundBar);
	BT_SoundOn->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleSoundOff);
	BT_SoundOff->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleSoundOn);
}

void UPCRPauseUserWidget::HandleResumeClicked()
{
	if (APCREricaPlayerController* EricaPlayerController = Cast<APCREricaPlayerController>(GetOwningPlayer()))
	{
		EricaPlayerController->SetPause(false);

		RemoveFromParent();
	}
}

void UPCRPauseUserWidget::HandleRestartClicked()
{
	if (APCREricaPlayerController* EricaPlayerController = Cast<APCREricaPlayerController>(GetOwningPlayer()))
	{
		EricaPlayerController->SetPause(false);

		RemoveFromParent();
	}

	const UWorld* CurrentWorld = GetWorld();
	const FString CurrentLevelName = CurrentWorld->GetName();
	UGameplayStatics::OpenLevel(CurrentWorld, FName(CurrentLevelName));
}

void UPCRPauseUserWidget::HandleQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UPCRPauseUserWidget::HandleChangeSoundBar(float Value)
{
	PB_SoundBar->SetPercent(Value);
	CachedPCRGameInstance->SetMasterVolume(Value);
	HandleSoundImageOnOff();
}

void UPCRPauseUserWidget::HandleSoundOn()
{
	CachedPCRGameInstance->MasterVolumeOn();
	HandleSoundImageOnOff();
}

void UPCRPauseUserWidget::HandleSoundOff()
{
	CachedPCRGameInstance->MasterVolumeOff();
	HandleSoundImageOnOff();
}

void UPCRPauseUserWidget::HandleSoundImageOnOff()
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
