// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/PlayerController.h"
#include "PCREricaPlayerController.generated.h"

class UPCRSerinUserWidget;
class UPCRHUDUserWidget;
class UPCRPauseUserWidget;
class UPCRUIDataAsset;
class APCREricaCharacter;
class UPCREricaDataAsset;
class APCRSerinDollHeadCharacter;
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
	virtual void PostInitializeComponents() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

public: // 동작 섹션
	FVector GetMouseDirection() const;
	void BindSerinUI(APCRSerinDollHeadCharacter* Serin);
	void SetVisibilityHUD(bool bIsEnable);
	void SetVisibilitySerinUI(bool bIsEnable);

private: // 내부 함수 섹션
	void BindEricaUI();
	void BindStage1UI();
	void GamePause();
	void GameOver();

private: // 데이터 에셋 섹션
	UPROPERTY()
	TObjectPtr<const UPCREricaDataAsset> EricaDataAsset;

	UPROPERTY()
	TObjectPtr<const UPCRUIDataAsset> UIDataAsset;

private: // 캐시 섹션
	UPROPERTY()
	TObjectPtr<APCREricaCharacter> CachedEricaCharacter;

private: // UI 섹션
	UPROPERTY()
	TSubclassOf<UPCRHUDUserWidget> HUDUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UPCRHUDUserWidget> HUDUserWidget;

	UPROPERTY()
	TSubclassOf<UPCRPauseUserWidget> PauseUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UPCRPauseUserWidget> PauseUserWidget;

	UPROPERTY()
	TSubclassOf<UPCRSerinUserWidget> SerinUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UPCRSerinUserWidget> SerinUserWidget;

	UPROPERTY()
	TSubclassOf<UUserWidget> GameOverUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> GameOverUserWidget;


private: // 상태 섹션
	uint32 bUseCharacterRotationByCursorDirection : 1;
};
