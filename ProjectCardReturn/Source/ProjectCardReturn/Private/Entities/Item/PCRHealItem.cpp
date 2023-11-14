// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRHealItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

APCRHealItem::APCRHealItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->HealItemEffect);
	}
}

void APCRHealItem::PlayerOverlapEvent(APCREricaCharacter* Player)
{
	Super::PlayerOverlapEvent(Player);

	//Player->ChangeHP(Player->GetMaxHP() * 0.1f);
}
