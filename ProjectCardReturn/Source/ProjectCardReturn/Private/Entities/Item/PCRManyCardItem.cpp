// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRManyCardItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

DEFINE_LOG_CATEGORY(PCRManyCardItem);

APCRManyCardItem::APCRManyCardItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->ManyCardItemEffect);
	}
}

void APCRManyCardItem::PlayerOverlapEvent()
{
	Super::PlayerOverlapEvent();

	Player->IncreaseMaxCardCount(3);
	UE_LOG(PCRManyCardItem, Warning, TEXT("카드 최대 수 : %d"), Player->GetMaxCardCount());
}
