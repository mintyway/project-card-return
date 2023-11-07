// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRFastShootItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"

APCRFastShootItem::APCRFastShootItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->FastShootItemEffect);
	}
}
