// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
};
