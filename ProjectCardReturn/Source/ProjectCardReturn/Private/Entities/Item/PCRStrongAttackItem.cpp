// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRStrongAttackItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"

APCRStrongAttackItem::APCRStrongAttackItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->StrongAttackItemEffect);
	}
}
