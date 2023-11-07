// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRStage1UserWidget.h"

#include "Components/TextBlock.h"

void UPCRStage1UserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FName TB_MaxMonsterCountKey = TEXT("TB_MaxMonsterCount");
	MaxMonsterCount = Cast<UTextBlock>(GetWidgetFromName(TB_MaxMonsterCountKey));
	check(MaxMonsterCount);
	
	const FName TB_CurrentMonsterCountKey = TEXT("TB_CurrentMonsterCount");
	CurrentMonsterCount = Cast<UTextBlock>(GetWidgetFromName(TB_CurrentMonsterCountKey));
	check(CurrentMonsterCount);
}

void UPCRStage1UserWidget::HandleUpdateMonsterCount(int32 InMaxMonsterCount, int32 InCurrentMonsterCount)
{

	const FString MaxMonsterCountString = FString::Printf(TEXT("%d"), InMaxMonsterCount);
	const FString CurrentMonsterCountString = FString::Printf(TEXT("%d"), InCurrentMonsterCount);
	const FText MaxMonsterCountText = FText::FromString(MaxMonsterCountString);
	const FText CurrentMonsterCountText = FText::FromString(CurrentMonsterCountString);
	MaxMonsterCount->SetText(MaxMonsterCountText);
	CurrentMonsterCount->SetText(CurrentMonsterCountText);
}
