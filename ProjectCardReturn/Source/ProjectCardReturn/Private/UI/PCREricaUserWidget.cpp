// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCREricaUserWidget.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/PCRSerinUserWidget.h"

void UPCREricaUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FName PB_HPKey = TEXT("PB_HP");
	HPBar = Cast<UProgressBar>(GetWidgetFromName(PB_HPKey));
	check(HPBar);

	const FName TB_CurrentCardCountKey = TEXT("TB_CurrentCardCount");
	CurrentCardCountTextBlock = Cast<UTextBlock>(GetWidgetFromName(TB_CurrentCardCountKey));
	check(CurrentCardCountTextBlock);

	const FName TB_MaxCardCountKey = TEXT("TB_MaxCardCount");
	MaxCardCountTextBlock = Cast<UTextBlock>(GetWidgetFromName(TB_MaxCardCountKey));
	check(MaxCardCountTextBlock);

	const FName IMG_NarrowShotKey = TEXT("IMG_NarrowShot");
	NarrowShotImage = Cast<UImage>(GetWidgetFromName(IMG_NarrowShotKey));
	check(NarrowShotImage);

	const FName IMG_WideShotKey = TEXT("IMG_WideShot");
	WideShotImage = Cast<UImage>(GetWidgetFromName(IMG_WideShotKey));
	check(WideShotImage);

	NarrowShotImage->SetVisibility(ESlateVisibility::Visible);
	WideShotImage->SetVisibility(ESlateVisibility::Hidden);
}

void UPCREricaUserWidget::HandleUpdateHP(float MaxHP, float CurrentHP)
{
	const float HPRatio = CurrentHP / MaxHP;
	HPBar->SetPercent(HPRatio);
}

void UPCREricaUserWidget::HandleUpdateCardCount(int32 MaxCardCount, int32 CurrentCardCount)
{
	const FString MaxCardCountString = FString::Printf(TEXT("%d"), MaxCardCount);
	const FString CurrentCardCountString = FString::Printf(TEXT("%d"), CurrentCardCount);
	const FText MaxCardCountText = FText::FromString(MaxCardCountString);
	const FText CurrentCardCardCountText = FText::FromString(CurrentCardCountString);
	MaxCardCountTextBlock->SetText(MaxCardCountText);
	CurrentCardCountTextBlock->SetText(CurrentCardCardCountText);
}

void UPCREricaUserWidget::HandleUpdateChangeShootMode(EShootMode InShootMode)
{
	switch (InShootMode)
	{
		case EShootMode::NarrowShot:
		{
			NarrowShotImage->SetVisibility(ESlateVisibility::Visible);
			WideShotImage->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
		case EShootMode::WideShot:
		{
			NarrowShotImage->SetVisibility(ESlateVisibility::Hidden);
			WideShotImage->SetVisibility(ESlateVisibility::Visible);
			break;
		}
	}
}