// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRMoreHpItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"

APCRMoreHpItem::APCRMoreHpItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->MoreHpItemEffect);
	}
}
