// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Blueprint/UserWidget.h"
#include "PCRHUDUserWidget.generated.h"

class UPCRStage1UserWidget;
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

public: // 동작
	void SetVisibilityEricaUI(bool bIsEnable);
	void SetVisibilityStage1UI(bool bIsEnable);
	void SetVisibilitySerinUI(bool bIsEnable);

public:
	UPROPERTY()
	TObjectPtr<UPCREricaUserWidget> EricaUserWidget;

	UPROPERTY()
	TObjectPtr<UPCRStage1UserWidget> Stage1UserWidget;
	
	UPROPERTY()
	TObjectPtr<UPCRSerinUserWidget> SerinUserWidget;
};
