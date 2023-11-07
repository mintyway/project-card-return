// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRHUDUserWidget.h"

#include "UI/PCREricaUserWidget.h"
#include "UI/PCRSerinUserWidget.h"
#include "UI/PCRStage1UserWidget.h"

void UPCRHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FName WB_EricaUIKey = TEXT("WB_EricaUI");
	EricaUserWidget = Cast<UPCREricaUserWidget>(GetWidgetFromName(WB_EricaUIKey));
	check(EricaUserWidget);

	const FName WB_Stage1UIKey = TEXT("WB_Stage1UI");
	Stage1UserWidget = Cast<UPCRStage1UserWidget>(GetWidgetFromName(WB_Stage1UIKey));
	check(Stage1UserWidget);
		
	const FName WB_SerinUIKey = TEXT("WB_SerinUI");
	SerinUserWidget = Cast<UPCRSerinUserWidget>(GetWidgetFromName(WB_SerinUIKey));
	check(SerinUserWidget);

	EricaUserWidget->SetVisibility(ESlateVisibility::Hidden);
	Stage1UserWidget->SetVisibility(ESlateVisibility::Hidden);
	SerinUserWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UPCRHUDUserWidget::SetVisibilityEricaUI(bool bIsEnable)
{
	if (bIsEnable)
	{
		EricaUserWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		EricaUserWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPCRHUDUserWidget::SetVisibilityStage1UI(bool bIsEnable)
{
	if (bIsEnable)
	{
		Stage1UserWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Stage1UserWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPCRHUDUserWidget::SetVisibilitySerinUI(bool bIsEnable)
{
	if (bIsEnable)
	{
		SerinUserWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SerinUserWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
