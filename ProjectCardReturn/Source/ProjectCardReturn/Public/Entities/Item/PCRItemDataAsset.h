// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PCRItemDataAsset.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraComponent> SpeedUpItemEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraComponent> MoreHpItemEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraComponent> ManyCardItemEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraComponent> HealItemEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraComponent> FastShootItemEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraComponent> LongRangeItemEffect;
};
