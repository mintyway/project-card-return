// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRLongerRangeItem.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Game/PCRParameterDataAsset.h"

DEFINE_LOG_CATEGORY(PCRLongerRangeItem);

APCRLongerRangeItem::APCRLongerRangeItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->LongRangeItemEffect);
	}
}

void APCRLongerRangeItem::PlayerOrCardOverlapEvent()
{
	Super::PlayerOrCardOverlapEvent();

	Player->IncreaseShootRange(ParameterDataAsset->ShootRangeIncreaseDistance);
	UE_LOG(PCRLongerRangeItem, Log, TEXT("카드 평균 사거리 : %.2f"), Player->CardAverageRange());

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ItemDataAsset->LongRangeItemPickEffect, Player->GetActorLocation(), GetActorRotation());
}
