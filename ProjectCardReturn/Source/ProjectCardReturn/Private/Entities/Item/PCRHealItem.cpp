// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRHealItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

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

	Player->Heal(Player->GetMaxHP() * 0.1f);
	UE_LOG(PCRHealItem, Warning, TEXT("체력 : %.2f"), Player->GetCurrentHP());
}
