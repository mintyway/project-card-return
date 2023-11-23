// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/RangedSoldier/EliteRangedSoldier/PCREliteRangedSoldierCharacter.h"

#include "Game/PCRParameterDataAsset.h"

APCREliteRangedSoldierCharacter::APCREliteRangedSoldierCharacter()
{
	if (ParameterDataAsset)
	{
		MaxHP = ParameterDataAsset->RangedSoldierMaxHP * 1.5f;
		CurrentHP = MaxHP;
		MoveSpeed = ParameterDataAsset->RangedSoldierMoveSpeed * 1.3f;
		AttackPower = ParameterDataAsset->RangedSoldierAttackPower * 1.5f;
	}

	bIsElite = true;
	
	if (GetMesh() && MonsterDataAsset)
	{
		GetMesh()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	}
}
