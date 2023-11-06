// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Blueprint/UserWidget.h"
#include "PCRMainUserWidget.generated.h"

class UImage;
enum class EShootMode : uint8;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void HandleUpdateHP(float MaxHP, float CurrentHP);
	void HandleUpdateCardCount(int32 MaxCardCount, int32 CurrentCardCount);
	void HandleUpdateChangeShootMode(EShootMode InShootMode);

	UPROPERTY()
	TObjectPtr<UProgressBar> HPBar;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> MaxCardCountTextBlock;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> CurrentCardCountTextBlock;

	UPROPERTY()
	TObjectPtr<UImage> NarrowShotImage;

	UPROPERTY()
	TObjectPtr<UImage> WideShotImage;
};
