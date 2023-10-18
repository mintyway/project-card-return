// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/PCRRangedSoldierCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/MeleeSoldier/PCRSpearActor.h"
#include "Game/PCRParameterDataAsset.h"

APCRRangedSoldierCharacter::APCRRangedSoldierCharacter()
{
	AttackType = EAttackType::Ranged;
	bHasSpear = false;
	
	if (IsValid(GetParameterDataAsset()))
	{
		AttackRange = GetParameterDataAsset()->RangedSoldierAttackRange;
	}
	
	// TODO: 모델링 작업 완료되면 활성화
	if (GetMesh() && GetMonsterDataAsset())
	{
		GetMesh()->SetSkeletalMesh(GetMonsterDataAsset()->RangedSoldierMesh);
	}
}

void APCRRangedSoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpawnAndAttachSpear();
}
