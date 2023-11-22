// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRUIDataAsset.generated.h"

class UPCRSerinUserWidget;
class UPCRHUDUserWidget;
class UPCRPauseUserWidget;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRUIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> HPBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<UPCRHUDUserWidget> HUDUserWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<UPCRPauseUserWidget> PauseUserWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> GameOverWidgetClass;
};
