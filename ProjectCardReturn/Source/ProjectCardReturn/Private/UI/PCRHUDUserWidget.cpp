// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRHUDUserWidget.h"

#include "UI/PCREricaUserWidget.h"
#include "UI/PCRSerinUserWidget.h"

void UPCRHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FName WB_EricaUIKey = TEXT("WB_EricaUI");
	EricaUserWidget = Cast<UPCREricaUserWidget>(GetWidgetFromName(WB_EricaUIKey));
	check(EricaUserWidget);
		
	const FName WB_SerinUIKey = TEXT("WB_SerinUI");
	SerinUserWidget = Cast<UPCRSerinUserWidget>(GetWidgetFromName(WB_SerinUIKey));
	check(SerinUserWidget);

	EricaUserWidget->SetVisibility(ESlateVisibility::Hidden);
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
