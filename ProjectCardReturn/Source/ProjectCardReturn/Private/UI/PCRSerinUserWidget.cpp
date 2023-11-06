// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRSerinUserWidget.h"

#include "Components/ProgressBar.h"

void UPCRSerinUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FName HPProgressBarKey = TEXT("HPProgressBar");
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(HPProgressBarKey));
	check(HPProgressBar);
}

void UPCRSerinUserWidget::HandleUpdateHP(float MaxHP, float CurrentHP)
{
	const float HPRatio = CurrentHP / MaxHP;
	HPProgressBar->SetPercent(HPRatio);
}
