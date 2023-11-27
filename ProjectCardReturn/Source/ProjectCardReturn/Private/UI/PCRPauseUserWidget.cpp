// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRPauseUserWidget.h"

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

	BT_ToMain = Cast<UButton>(GetWidgetFromName(TEXT("BT_ToMain")));
	check(BT_ToMain);

	CachedPCRGameInstance = Cast<UPCRGameInstance>(GetGameInstance());
	check(CachedPCRGameInstance);

	BT_Resume->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleResumeClicked);
	BT_Restart->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleRestartClicked);
	BT_ToMain->OnClicked.AddDynamic(this, &UPCRPauseUserWidget::HandleToMainClicked);
}

void UPCRPauseUserWidget::HandleResumeClicked()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwningPlayer()))
	{
		PlayerController->SetPause(false);

		RemoveFromParent();
	}
}

void UPCRPauseUserWidget::HandleRestartClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();
	check(PlayerController);
	PlayerController->SetPause(false);

	CachedPCRGameInstance->RestartGame(this);
}

void UPCRPauseUserWidget::HandleToMainClicked()
{
	CachedPCRGameInstance->ToMain();
}