// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCardReturn.h"
#include "Engine/DataAsset.h"
#include "PCRProjectileDataAsset.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRProjectileDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Erica")
	TObjectPtr<UStaticMesh> EricaCardMesh;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TObjectPtr<UNiagaraSystem> CardRibbon;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TObjectPtr<UNiagaraSystem> CardFloating;
};
