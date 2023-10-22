// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRPauseUserWidget.h"

#include "Entities/Players/Erica/PCREricaPlayerController.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPCRPauseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("ResumeButton")));
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleResumeClicked);
	}

	RestartButton = Cast<UButton>(GetWidgetFromName(TEXT("RestartButton")));
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleRestartClicked);
	}

	QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitButton")));
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleQuitButtonClicked);
	}
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

void UPCRPauseUserWidget::HandleQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
