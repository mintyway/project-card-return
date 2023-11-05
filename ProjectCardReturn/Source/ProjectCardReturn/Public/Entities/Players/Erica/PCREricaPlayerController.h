// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "GameFramework/PlayerController.h"
#include "PCREricaPlayerController.generated.h"

class UPCRBossUserWidget;
class UPCRMainUserWidget;
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

private: // 내부 함수 섹션
	void GamePause();

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
	TSubclassOf<UPCRMainUserWidget> MainUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UPCRMainUserWidget> MainUserWidget;

	UPROPERTY()
	TSubclassOf<UPCRPauseUserWidget> PauseUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UPCRPauseUserWidget> PauseUserWidget;

	UPROPERTY()
	TSubclassOf<UPCRBossUserWidget> BossUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UPCRBossUserWidget> BossUserWidget;

private: // 상태 섹션
	uint32 bUseCharacterRotationByCursorDirection : 1;
};
