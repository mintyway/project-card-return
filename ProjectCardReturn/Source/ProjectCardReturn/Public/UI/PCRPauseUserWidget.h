// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Blueprint/UserWidget.h"
#include "PCRPauseUserWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRPauseUserWidget: public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void HandleResumeClicked();
	
	UFUNCTION()
	void HandleRestartClicked();
	
	UFUNCTION()
	void HandleQuitButtonClicked();
	
	UPROPERTY()
	TObjectPtr<UButton> ResumeButton;
	
	UPROPERTY()
	TObjectPtr<UButton> RestartButton;

	UPROPERTY()
	TObjectPtr<UButton> QuitButton;
};
