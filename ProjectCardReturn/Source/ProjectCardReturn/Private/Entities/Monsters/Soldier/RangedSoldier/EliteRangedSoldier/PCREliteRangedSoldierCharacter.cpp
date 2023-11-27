// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Soldier/RangedSoldier/EliteRangedSoldier/PCREliteRangedSoldierCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Game/PCRParameterDataAsset.h"

APCREliteRangedSoldierCharacter::APCREliteRangedSoldierCharacter()
{
	if (ParameterDataAsset)
	{
		const float MaxHpRate = ParameterDataAsset->EliteRangedSoldierMaxHpRate;
		const float MoveSpeedRate = ParameterDataAsset->EliteRangedSoldierMoveSpeedRate;
		const float AttackPowerRate = ParameterDataAsset->EliteRangedSoldierAttackPowerRate;
		const float ScaleRate = ParameterDataAsset->EliteRangedSoldierScaleRate;
	
		MaxHP = ParameterDataAsset->RangedSoldierMaxHP * MaxHpRate;
		CurrentHP = MaxHP;
		MoveSpeed = ParameterDataAsset->RangedSoldierMoveSpeed * MoveSpeedRate;
		AttackPower = ParameterDataAsset->RangedSoldierAttackPower * AttackPowerRate;

		if (GetCapsuleComponent())
		{
			const float Radius = GetCapsuleComponent()->GetScaledCapsuleRadius() * ScaleRate;
			const float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * ScaleRate;
			GetCapsuleComponent()->InitCapsuleSize(Radius, HalfHeight);
		}

		if (GetMesh())
		{
			GetMesh()->SetRelativeScale3D(FVector(ScaleRate, ScaleRate, ScaleRate));
		}
	}

	bIsElite = true;
}
