// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRSpeedUpItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"

APCRSpeedUpItem::APCRSpeedUpItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->SpeedUpItemEffect);
	}
}
