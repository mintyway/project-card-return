// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRMainUserWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPCRMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FName HPProgressBarKey = TEXT("HPProgressBar");
	verify((HPBar = Cast<UProgressBar>(GetWidgetFromName(HPProgressBarKey))));

	const FName CardCountTextBlockKey = TEXT("CardCountTextBlock");
	verify((CardCountTextBlock = Cast<UTextBlock>(GetWidgetFromName(CardCountTextBlockKey))));
}

void UPCRMainUserWidget::HandleUpdateHP(float MaxHP, float CurrentHP)
{
	const float HPRatio = CurrentHP / MaxHP;
	HPBar->SetPercent(HPRatio);
}

void UPCRMainUserWidget::HandleUpdateCardCount(int32 MaxCardCount, int32 CurrentCardCount)
{
	const FString CardCountString = FString::Printf(TEXT("%d/%d"), CurrentCardCount, MaxCardCount);
	const FText CardCountText = FText::FromString(CardCountString);
	CardCountTextBlock->SetText(CardCountText);
}
