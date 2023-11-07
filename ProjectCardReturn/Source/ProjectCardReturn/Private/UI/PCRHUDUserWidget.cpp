// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCRHUDUserWidget.h"

#include "UI/PCREricaUserWidget.h"
#include "UI/PCRSerinUserWidget.h"

void UPCRHUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FName WB_EricaUIKey = TEXT("WB_EricaUI");
	SerinUserWidget = Cast<UPCREricaUserWidget>(GetWidgetFromName(WB_EricaUIKey));
	check(SerinUserWidget);
		
	const FName WB_SerinUIKey = TEXT("WB_SerinUI");
	BossUserWidget = Cast<UPCRSerinUserWidget>(GetWidgetFromName(WB_SerinUIKey));
	check(BossUserWidget);
}
