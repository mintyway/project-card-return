// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/Monsters/Rabbit/EliteRabbit/PCREliteRabbitCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Game/PCRParameterDataAsset.h"

APCREliteRabbitCharacter::APCREliteRabbitCharacter()
{
	if (ParameterDataAsset)
	{
		const float MaxHpRate = ParameterDataAsset->EliteRabbitMaxHpRate;
		const float MoveSpeedRate = ParameterDataAsset->EliteRabbitMoveSpeedRate;
		const float AttackPowerRate = ParameterDataAsset->EliteRabbitAttackPowerRate;
		const float ScaleRate = ParameterDataAsset->EliteRabbitScaleRate;
	
		MaxHP = ParameterDataAsset->RabbitMaxHP * MaxHpRate;
		CurrentHP = MaxHP;
		MoveSpeed = ParameterDataAsset->RabbitMoveSpeed * MoveSpeedRate;
		AttackPower = ParameterDataAsset->RabbitAttackPower * AttackPowerRate;

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
