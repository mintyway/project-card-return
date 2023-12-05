// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRSoundPrimaryDataAsset.generated.h"

class UFMODEvent;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRSoundPrimaryDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "BGM")
	TObjectPtr<UFMODEvent> MainBGM;

	UPROPERTY(EditDefaultsOnly, Category = "BGM")
	TObjectPtr<UFMODEvent> AmbientBGM;

	UPROPERTY(EditDefaultsOnly, Category = "BGM")
	TObjectPtr<UFMODEvent> Stage1BGM;

	UPROPERTY(EditDefaultsOnly, Category = "BGM")
	TObjectPtr<UFMODEvent> BossStageBGM;

	UPROPERTY(EditDefaultsOnly, Category = "BGM")
	TObjectPtr<UFMODEvent> EndingBGM;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	TObjectPtr<UFMODEvent> GameOver;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	TObjectPtr<UFMODEvent> LightOn;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UFMODEvent> Button;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	TObjectPtr<UFMODEvent> Shot;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	TObjectPtr<UFMODEvent> BlockedCard;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	TObjectPtr<UFMODEvent> ReturnCard;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	TObjectPtr<UFMODEvent> Dash;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	TObjectPtr<UFMODEvent> Damaged;

	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	TObjectPtr<UFMODEvent> Move;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stage")
	TObjectPtr<UFMODEvent> StageChange;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	TObjectPtr<UFMODEvent> Rock;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	TObjectPtr<UFMODEvent> Paper;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	TObjectPtr<UFMODEvent> Scissors;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	TObjectPtr<UFMODEvent> Pattern1Bomb;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	TObjectPtr<UFMODEvent> Pattern1BombCrash;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	TObjectPtr<UFMODEvent> Pattern1BombThrow;

	UPROPERTY(EditDefaultsOnly, Category = "Serin")
	TObjectPtr<UFMODEvent> Pattern1BombTimer;

};
