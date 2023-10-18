// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/MeleeSoldier/PCRMeleeSoldierCharacter.h"

#include "Entities/Monsters/Base/PCRMonsterDataAsset.h"
#include "Entities/Monsters/MeleeSoldier/PCRShieldActor.h"
#include "Game/PCRParameterDataAsset.h"

APCRMeleeSoldierCharacter::APCRMeleeSoldierCharacter()
{
	AttackType = EAttackType::Melee;
	bHasShield = false;

	if (IsValid(GetParameterDataAsset()))
	{
		AttackRange = GetParameterDataAsset()->MeleeSoldierAttackRange;
	}
	
	// TODO: 모델링 작업 완료되면 활성화
	if (GetMesh() && GetMonsterDataAsset())
	{
		GetMesh()->SetSkeletalMesh(GetMonsterDataAsset()->MeleeSoldierMesh);
	}
}

void APCRMeleeSoldierCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SpawnAndAttachShield();
	SpawnAndAttachSpear();
}
