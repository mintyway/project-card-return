// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Blueprint/UserWidget.h"
#include "PCRBossUserWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRBossUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void HandleUpdateHP(float MaxHP, float CurrentHP);

private:
	TObjectPtr<UProgressBar> HPProgressBar;
};
