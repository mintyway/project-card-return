// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/MeleeSoldier/EliteMeleeSoldier/PCREliteMeleeSoldierCharacter.h"

#include "Game/PCRParameterDataAsset.h"

APCREliteMeleeSoldierCharacter::APCREliteMeleeSoldierCharacter()
{
	if (ParameterDataAsset)
	{
		const float MaxHpRate = ParameterDataAsset->EliteMeleeSoldierMaxHpRate;
		const float MoveSpeedRate = ParameterDataAsset->EliteMeleeSoldierMoveSpeedRate;
		const float AttackPowerRate = ParameterDataAsset->EliteMeleeSoldierAttackPowerRate;
		const float ScaleRate = ParameterDataAsset->EliteMeleeSoldierScaleRate;
		
		MaxHP = ParameterDataAsset->MeleeSoldierMaxHP * MaxHpRate;
		CurrentHP = MaxHP;
		MoveSpeed = ParameterDataAsset->MeleeSoldierMoveSpeed * MoveSpeedRate;
		AttackPower = ParameterDataAsset->MeleeSoldierAttackPower * AttackPowerRate;

		if (GetMesh())
		{
			GetMesh()->SetRelativeScale3D(FVector(ScaleRate, ScaleRate, ScaleRate));
		}
	}

	bIsElite = true;
}
