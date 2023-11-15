// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRStrongAttackItem.h"

#include "NiagaraComponent.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"

DEFINE_LOG_CATEGORY(PCRStrongAttackItem);

APCRStrongAttackItem::APCRStrongAttackItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->StrongAttackItemEffect);
	}
}

void APCRStrongAttackItem::PlayerOverlapEvent()
{
	Super::PlayerOverlapEvent();

	Player->IncreaseDamage(0.5f);
	UE_LOG(PCRStrongAttackItem, Warning, TEXT("카드 평균 데미지 : %.2f"), Player->CardAverageDamage());
}
