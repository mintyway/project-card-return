// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/PlayerController.h"
#include "PCREricaPlayerController.generated.h"

class APCREricaCharacter;
class UPCREricaDataAsset;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API APCREricaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APCREricaPlayerController();

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
	void HandleMenuInput();
	void Test();

	UPROPERTY()
	TObjectPtr<const UPCREricaDataAsset> DataAsset;

	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedEricaCharacter;
};
