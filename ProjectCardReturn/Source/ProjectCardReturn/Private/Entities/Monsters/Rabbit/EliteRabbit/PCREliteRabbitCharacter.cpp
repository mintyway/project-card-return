// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/EliteRabbit/PCREliteRabbitCharacter.h"

#include "Game/PCRParameterDataAsset.h"

APCREliteRabbitCharacter::APCREliteRabbitCharacter()
{
	const float MaxHpRate = ParameterDataAsset->EliteRabbitMaxHpRate;
	const float MoveSpeedRate = ParameterDataAsset->EliteRabbitMoveSpeedRate;
	const float AttackPowerRate = ParameterDataAsset->EliteRabbitAttackPowerRate;
	const float ScaleRate = ParameterDataAsset->EliteRabbitScaleRate;
	
	if (ParameterDataAsset)
	{
		MaxHP = ParameterDataAsset->RabbitMaxHP * MaxHpRate;
		CurrentHP = MaxHP;
		MoveSpeed = ParameterDataAsset->RabbitMoveSpeed * MoveSpeedRate;
		AttackPower = ParameterDataAsset->RabbitAttackPower * AttackPowerRate;
	}

	bIsElite = true;
	
	if (GetMesh() && MonsterDataAsset)
	{
		GetMesh()->SetRelativeScale3D(FVector(ScaleRate, ScaleRate, ScaleRate));
	}
}
