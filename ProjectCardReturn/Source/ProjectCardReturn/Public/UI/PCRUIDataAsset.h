// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRUIDataAsset.generated.h"

class UPCRBossUserWidget;
class UPCRMainUserWidget;
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
	TSoftClassPtr<UPCRMainUserWidget> Main;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<UPCRPauseUserWidget> Pause;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<UPCRBossUserWidget> Boss;
};
