// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/MeleeSoldier/EliteMeleeSoldier/PCREliteMeleeSoldierCharacter.h"

#include "Game/PCRParameterDataAsset.h"

APCREliteMeleeSoldierCharacter::APCREliteMeleeSoldierCharacter()
{
	if (ParameterDataAsset)
	{
		MaxHP = ParameterDataAsset->MeleeSoldierMaxHP * 2.0f;
		CurrentHP = MaxHP;
		AttackPower = ParameterDataAsset->MeleeSoldierAttackPower * 1.5f;
	}

	bIsElite = true;
	
	if (GetMesh() && MonsterDataAsset)
	{
		GetMesh()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	}
}
