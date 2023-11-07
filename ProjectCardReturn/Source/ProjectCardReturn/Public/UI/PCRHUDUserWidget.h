// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Blueprint/UserWidget.h"
#include "PCRHUDUserWidget.generated.h"

class UPCREricaUserWidget;
class UPCRSerinUserWidget;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRHUDUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:

	UPROPERTY()
	TObjectPtr<UPCREricaUserWidget> SerinUserWidget;
	
	UPROPERTY()
	TObjectPtr<UPCRSerinUserWidget> BossUserWidget;
};
