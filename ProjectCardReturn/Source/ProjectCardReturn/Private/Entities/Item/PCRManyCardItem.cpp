// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRManyCardItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"

APCRManyCardItem::APCRManyCardItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->ManyCardItemEffect);
	}
}
