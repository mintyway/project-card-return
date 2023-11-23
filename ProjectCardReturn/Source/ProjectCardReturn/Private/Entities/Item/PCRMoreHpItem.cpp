// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRMoreHpItem.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Game/PCRParameterDataAsset.h"

DEFINE_LOG_CATEGORY(PCRMoreHpItem);

APCRMoreHpItem::APCRMoreHpItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->MoreHpItemEffect);
	}
}

void APCRMoreHpItem::PlayerOrCardOverlapEvent()
{
	Super::PlayerOrCardOverlapEvent();

	Player->IncreaseMaxHP(Player->GetMaxHP() * ParameterDataAsset->MaxHPIncreaseRate);
	Player->Heal(Player->GetCurrentHP() * ParameterDataAsset->HealRateByIncreaseMaxHP);
	UE_LOG(PCRMoreHpItem, Log, TEXT("체력 최대치 : %.2f"), Player->GetMaxHP());

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ItemDataAsset->MoreHpItemPickEffect, Player->GetActorLocation(), GetActorRotation());
}
