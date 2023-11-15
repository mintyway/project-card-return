// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PCRItemDataAsset.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PROJECTCARDRETURN_API UPCRItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraSystem> SpeedUpItemEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraSystem> MoreHpItemEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraSystem> ManyCardItemEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraSystem> HealItemEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraSystem> FastShootItemEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraSystem> StrongAttackItemEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TObjectPtr<UNiagaraSystem> LongRangeItemEffect;
};
