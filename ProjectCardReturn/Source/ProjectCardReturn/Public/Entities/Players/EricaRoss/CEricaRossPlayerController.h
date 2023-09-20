// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/PlayerController.h"
#include "CEricaRossPlayerController.generated.h"

class ACEricaRossCharacter;
class UCEricaRossDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API ACEricaRossPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACEricaRossPlayerController();

protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

private:
	void Shoot();
	void Return();
	
	TObjectPtr<UCEricaRossDataAsset> DataAsset;
	TObjectPtr<ACEricaRossCharacter> CachedEricaRossCharacter; 
};
