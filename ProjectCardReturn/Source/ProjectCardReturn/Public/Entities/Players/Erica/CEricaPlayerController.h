// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/PlayerController.h"
#include "CEricaPlayerController.generated.h"

class ACEricaCharacter;
class UCEricaDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API ACEricaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACEricaPlayerController();

protected:
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	FVector GetMouseDirection() const;

private:
	void Shoot();
	void Return();

	UPROPERTY()
	TObjectPtr<const UCEricaDataAsset> DataAsset;

	UPROPERTY()
	TObjectPtr<ACEricaCharacter> CachedEricaCharacter;
};
