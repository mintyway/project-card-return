// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRLongerRangeItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"

APCRLongerRangeItem::APCRLongerRangeItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->LongRangeItemEffect);
	}
}
