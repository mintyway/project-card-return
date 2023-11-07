// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRSerinUserWidget.h"

#include "Components/ProgressBar.h"

void UPCRSerinUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FName PB_HPKey = TEXT("PB_HP");
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(PB_HPKey));
	check(HPProgressBar);
}

void UPCRSerinUserWidget::HandleUpdateHP(float MaxHP, float CurrentHP)
{
	const float HPRatio = CurrentHP / MaxHP;
	HPProgressBar->SetPercent(HPRatio);
}
