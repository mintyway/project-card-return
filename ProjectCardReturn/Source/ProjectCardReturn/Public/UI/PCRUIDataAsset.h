// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRUIDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRUIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FORCEINLINE TSoftClassPtr<UUserWidget> GetHPBar() const { return HPBar; }
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<UUserWidget> HPBar;
};
