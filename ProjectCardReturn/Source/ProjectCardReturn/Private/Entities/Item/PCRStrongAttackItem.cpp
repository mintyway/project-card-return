// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Item/PCRStrongAttackItem.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Entities/Item/PCRItemDataAsset.h"
#include "Entities/Players/Erica/PCREricaCharacter.h"
#include "Game/PCRParameterDataAsset.h"

DEFINE_LOG_CATEGORY(PCRStrongAttackItem);

APCRStrongAttackItem::APCRStrongAttackItem()
{
	if (NiagaraComponent && ItemDataAsset)
	{
		NiagaraComponent->SetAsset(ItemDataAsset->StrongAttackItemEffect);
	}
}

void APCRStrongAttackItem::PlayerOrCardOverlapEvent()
{
	Super::PlayerOrCardOverlapEvent();

	Player->IncreaseDamage(ParameterDataAsset->DamageIncreaseValue);
	UE_LOG(PCRStrongAttackItem, Log, TEXT("카드 평균 데미지 : %.2f"), Player->CardAverageDamage());

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ItemDataAsset->StrongAttackItemPickEffect, Player->GetActorLocation(), GetActorRotation());
}
