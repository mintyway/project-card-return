// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/PlayerController.h"
#include "PCREricaPlayerController.generated.h"

class UPCRPauseUserWidget;
class UPCRUIDataAsset;
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
	void EnableUIInputMode();
	void DisableUIInputMode();

private:
	void Shoot();
	void Return();
	void GamePause();

	UPROPERTY()
	TObjectPtr<const UPCREricaDataAsset> EricaDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRUIDataAsset> UIDataAsset;

	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedEricaCharacter;

	UPROPERTY()
	TSubclassOf<UPCRPauseUserWidget> PauseUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UPCRPauseUserWidget> PauseUserWidget;

	uint32 bUseCharacterRotationByCursorDirection:1;
};
