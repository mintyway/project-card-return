// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Blueprint/UserWidget.h"
#include "PCRStage1UserWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRStage1UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void HandleUpdateMonsterCount(int32 InMaxMonsterCount, int32 InCurrentMonsterCount);

private:
	UPROPERTY()
	TObjectPtr<UTextBlock> MaxMonsterCount;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> CurrentMonsterCount;
};
