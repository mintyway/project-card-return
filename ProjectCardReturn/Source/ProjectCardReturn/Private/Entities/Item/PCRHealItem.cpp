// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRHealItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"

APCRHealItem::APCRHealItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->HealItemEffect);
	}
}
