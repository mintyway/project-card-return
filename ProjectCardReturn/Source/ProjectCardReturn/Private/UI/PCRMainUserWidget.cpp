// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRMainUserWidget.h"

#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/PCRSerinUserWidget.h"

void UPCRMainUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FName WB_SerinUIKey = TEXT("WB_SerinUI");
	BossUserWidget = Cast<UPCRSerinUserWidget>(GetWidgetFromName(WB_SerinUIKey));
	check(BossUserWidget);
	
	const FName HPProgressBarKey = TEXT("HPProgressBar");
	HPBar = Cast<UProgressBar>(GetWidgetFromName(HPProgressBarKey));
	check(HPBar);

	const FName CurrentCardCountTextBlockKey = TEXT("CurrentCardCountTextBlock");
	CurrentCardCountTextBlock = Cast<UTextBlock>(GetWidgetFromName(CurrentCardCountTextBlockKey));
	check(CurrentCardCountTextBlock);

	const FName MaxCardCountTextBlockKey = TEXT("MaxCardCountTextBlock");
	MaxCardCountTextBlock = Cast<UTextBlock>(GetWidgetFromName(MaxCardCountTextBlockKey));
	check(MaxCardCountTextBlock);

	const FName NarrowShotImageKey = TEXT("NarrowShotImage");
	NarrowShotImage = Cast<UImage>(GetWidgetFromName(NarrowShotImageKey));
	check(NarrowShotImage);

	const FName WideShotImageKey = TEXT("WideShotImage");
	WideShotImage = Cast<UImage>(GetWidgetFromName(WideShotImageKey));
	check(WideShotImage);

	NarrowShotImage->SetVisibility(ESlateVisibility::Visible);
	WideShotImage->SetVisibility(ESlateVisibility::Hidden);
}

void UPCRMainUserWidget::HandleUpdateHP(float MaxHP, float CurrentHP)
{
	const float HPRatio = CurrentHP / MaxHP;
	HPBar->SetPercent(HPRatio);
}

void UPCRMainUserWidget::HandleUpdateCardCount(int32 MaxCardCount, int32 CurrentCardCount)
{
	const FString MaxCardCountString = FString::Printf(TEXT("%d"), MaxCardCount);
	const FString CurrentCardCountString = FString::Printf(TEXT("%d"), CurrentCardCount);
	const FText MaxCardCountText = FText::FromString(MaxCardCountString);
	const FText CurrentCardCardCountText = FText::FromString(CurrentCardCountString);
	MaxCardCountTextBlock->SetText(MaxCardCountText);
	CurrentCardCountTextBlock->SetText(CurrentCardCardCountText);
}

void UPCRMainUserWidget::HandleUpdateChangeShootMode(EShootMode InShootMode)
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
