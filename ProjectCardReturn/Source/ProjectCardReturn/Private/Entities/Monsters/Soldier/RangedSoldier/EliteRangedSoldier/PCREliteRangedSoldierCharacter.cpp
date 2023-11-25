// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/RangedSoldier/EliteRangedSoldier/PCREliteRangedSoldierCharacter.h"

#include "Game/PCRParameterDataAsset.h"

APCREliteRangedSoldierCharacter::APCREliteRangedSoldierCharacter()
{
	const float MaxHpRate = ParameterDataAsset->EliteRangedSoldierMaxHpRate;
	const float MoveSpeedRate = ParameterDataAsset->EliteRangedSoldierMoveSpeedRate;
	const float AttackPowerRate = ParameterDataAsset->EliteRangedSoldierAttackPowerRate;
	const float ScaleRate = ParameterDataAsset->EliteRangedSoldierScaleRate;
	
	if (ParameterDataAsset)
	{
		MaxHP = ParameterDataAsset->RangedSoldierMaxHP * MaxHpRate;
		CurrentHP = MaxHP;
		MoveSpeed = ParameterDataAsset->RangedSoldierMoveSpeed * MoveSpeedRate;
		AttackPower = ParameterDataAsset->RangedSoldierAttackPower * AttackPowerRate;
	}

	bIsElite = true;
	
	if (GetMesh() && MonsterDataAsset)
	{
		GetMesh()->SetRelativeScale3D(FVector(ScaleRate, ScaleRate, ScaleRate));
	}
}
