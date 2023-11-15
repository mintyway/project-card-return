// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRHealItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Game/PCRParameterDataAsset.h"

DEFINE_LOG_CATEGORY(PCRHealItem);

APCRHealItem::APCRHealItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->HealItemEffect);
	}
}

void APCRHealItem::PlayerOverlapEvent()
{
	Super::PlayerOverlapEvent();

	Player->Heal(Player->GetMaxHP() * ParameterDataAsset->HealRate);
	UE_LOG(PCRHealItem, Log, TEXT("체력 : %.2f"), Player->GetCurrentHP());
}
