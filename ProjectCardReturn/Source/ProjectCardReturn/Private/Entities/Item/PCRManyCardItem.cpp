// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRManyCardItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Game/PCRParameterDataAsset.h"

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

	Player->IncreaseMaxCardCount(ParameterDataAsset->MaxCardIncreaseCount);
	UE_LOG(PCRManyCardItem, Log, TEXT("카드 최대 수 : %d"), Player->GetMaxCardCount());
}
