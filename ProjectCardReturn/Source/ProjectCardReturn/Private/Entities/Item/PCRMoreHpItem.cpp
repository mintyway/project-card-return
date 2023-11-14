// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRMoreHpItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

APCRMoreHpItem::APCRMoreHpItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->MoreHpItemEffect);
	}
}

void APCRMoreHpItem::PlayerOverlapEvent()
{
	Super::PlayerOverlapEvent();

	const float MaxHP = Player->GetMaxHP() + Player->GetMaxHP() * 0.05f;
	Player->SetMaxHP(MaxHP);
	Player->Heal(Player->GetCurrentHP() * 0.05f);
}
